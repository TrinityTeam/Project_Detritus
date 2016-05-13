#include "application.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>
#include <Urho3D/Urho2D/CollisionChain2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Scene/Scene.h>
#include "character.hpp"
#include "obstacle_spawner.hpp"
#include "obstacle.hpp"



Application::Application(Urho3D::Context* context):
    Urho3D::Application {context} {

    context->RegisterFactory<Character>();
    context->RegisterFactory<ObstacleSpawner>();
    context->RegisterFactory<Obstacle>();
}



void Application::Setup() {
    engineParameters_["ResourcePrefixPaths"] = "../../data;";
    engineParameters_["ResourcePaths"]=".;CoreData;";
    engineParameters_["VSync"] = true;
    engineParameters_["TextureQuality"] = 15;
}



void Application::Start() {
    auto cache = GetSubsystem<Urho3D::ResourceCache>();

    engine_->SetMinFps(60);
    engine_->SetMaxFps(60);

    initUI();
    initScene();
    readLevelFromJSON(cache->GetResource<Urho3D::JSONFile>("Levels/Test.json"));

    SubscribeToEvent(Urho3D::E_KEYDOWN,
                     [this](Urho3D::StringHash,
                            Urho3D::VariantMap& eventParams) {
                         auto key = Urho3D::KeyDown::P_SCANCODE;
                         this->keyDown(eventParams[key].GetInt());
                     });
    SubscribeToEvent(Urho3D::E_UPDATE,
                     [this](Urho3D::StringHash,
                            Urho3D::VariantMap& eventParams) {
                         auto timestep = Urho3D::Update::P_TIMESTEP;
                         this->update(eventParams[timestep].GetFloat());
                     });
    SubscribeToEvent(Urho3D::E_POSTRENDERUPDATE,
                     [this](Urho3D::StringHash,
                            Urho3D::VariantMap&) {
                         this->scene->GetComponent<Urho3D::PhysicsWorld2D>()->DrawDebugGeometry();
                     });
}



void Application::Stop() {}



void Application::update(float) {
    auto uiRoot = GetSubsystem<Urho3D::UI>()->GetRoot();
    auto character = scene->GetChild("Character");

    Urho3D::String info =
            Urho3D::String {character->GetPosition2D()} + "\n" +
            Urho3D::String {character->GetComponent<Character>()->isInAir()} + "\n" +
            Urho3D::String {character->GetComponent<Character>()->getLinearVelocity()};
    auto text = dynamic_cast<Urho3D::Text*>(uiRoot->GetChild(Urho3D::String{"Text"}));
    text->SetText(info);
    auto input = GetSubsystem<Urho3D::Input>();
    if(input->GetScancodeDown(input->GetScancodeFromName("Escape"))) {
        engine_->Exit();
    }
}



void Application::keyDown(int scancode) {
    auto input = GetSubsystem<Urho3D::Input>();
    auto name = input->GetScancodeName(scancode);
    if(name == "Escape") {
        engine_->Exit();
    }
}



void Application::initUI() {
    auto root = GetSubsystem<Urho3D::UI>()->GetRoot();
    auto cache = GetSubsystem<Urho3D::ResourceCache>();
    root->SetDefaultStyle(cache->GetResource
                          <Urho3D::XMLFile>("UI/DefaultStyle.xml"));
    auto text = root->CreateChild<Urho3D::Text>("Text");
    text->SetText("Hello, Urho!");
    text->SetPosition({-200, -200});
    text->SetColor({1, 0, 0});
    text->SetFont(cache->GetResource
                  <Urho3D::Font>("Fonts/Anonymous Pro.ttf"),15);
    text->SetTextAlignment(Urho3D::HA_CENTER);
    text->SetHorizontalAlignment(Urho3D::HA_CENTER);
    text->SetVerticalAlignment(Urho3D::VA_CENTER);
}



void Application::initScene() {
    scene = new Urho3D::Scene {GetContext()};
    scene->CreateComponent<Urho3D::Octree>();
    scene->CreateComponent<Urho3D::DebugRenderer>();
    scene->CreateComponent<Urho3D::PhysicsWorld2D>();

    GetSubsystem<Urho3D::Renderer>()->GetDefaultZone()->SetFogColor({1, 1, 1});

    auto spawnerNode = GetContext()->CreateObject<Urho3D::Node>();//scene->CreateChild("Spawner");
    spawnerNode->SetName("Spawner");
    auto spawner = spawnerNode->CreateComponent<ObstacleSpawner>();
    spawner->addObstacle<Obstacle>(2);
    spawner->addObstacle<Obstacle>(4);
    spawner->addObstacle<Obstacle>(6);
    spawner->start();

    scene->AddChild(spawnerNode);

    auto characterNode = scene->CreateChild("Character");

    cameraNode = characterNode->CreateChild("Camera");
    auto camera = cameraNode->CreateComponent<Urho3D::Camera>();
    camera->SetFarClip(500.0f);
    camera->SetOrthographic(true);
    camera->SetOrthoSize({16/9.*3, 3});

    Urho3D::SharedPtr<Urho3D::Viewport> viewport
        { new Urho3D::Viewport {GetContext(), scene, camera} };
    GetSubsystem<Urho3D::Renderer>()->SetViewport(0, viewport);
}



void Application::readLevelFromJSON(Urho3D::JSONFile* file) {
    auto root = file->GetRoot();
    if(scene.Null()) {
        initScene();
    }
    std::vector<Urho3D::Vector2> floorVertices {};
    for(auto& point: root.Get("floor").GetArray()) {
        floorVertices.push_back({point.Get("x").GetFloat(), point.Get("y").GetFloat()});
    }
    createFloor("Floor", floorVertices);
    for(auto& entity: root.Get("entities").GetArray()) {
        if(entity.Get("type").GetString() == "PC" ) {
            scene->GetChild("Character")->Translate2D({0, 10});
            scene->GetChild("Character")->CreateComponent<Character>();
        }
    }
}



void Application::createFloor(const std::string& name,
                              const std::vector<Urho3D::Vector2>& vertices) {
    auto floorNode = scene->CreateChild(name.c_str());
    Urho3D::RigidBody2D* floorBody =
            floorNode->CreateComponent<Urho3D::RigidBody2D>();
    floorBody->SetBodyType(Urho3D::BT_STATIC);
    Urho3D::CollisionChain2D* collisionPlane =
            floorNode->CreateComponent<Urho3D::CollisionChain2D>();
    collisionPlane->SetVertices({vertices.data(), vertices.size()});
    collisionPlane->SetFriction(1);
    floorBody->AddCollisionShape2D(collisionPlane);
}
