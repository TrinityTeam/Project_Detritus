#include "application.hpp"
#include "log.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
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
#include <Urho3D/Urho2D/ParticleEffect2D.h>
#include <Urho3D/Urho2D/ParticleEmitter2D.h>
#include "character.hpp"
#include "obstacle_spawner.hpp"
#include "obstacle.hpp"
#include "animation.hpp"
#include "sprite.hpp"
#include "creature.hpp"



Application::Application(Urho3D::Context* context):
    Urho3D::Application {context} {

    context->RegisterFactory<Character>();
    context->RegisterFactory<ObstacleSpawner>();
    context->RegisterFactory<Obstacle>();
    context->RegisterFactory<Sprite>();
    context->RegisterFactory<cAnimation>();
    context->RegisterFactory<Creature>();
}



void Application::Setup() {
    engineParameters_["ResourcePrefixPaths"] = "../../../data;";
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
                         /*auto debugRenderer = this->scene->GetComponent<Urho3D::DebugRenderer>();
                         debugRenderer->AddCross({0, 0}, 10, Urho3D::Color::RED);
                         debugRenderer->AddBoundingBox({{-0.5, -1}, {0.5, 0}}, Urho3D::Color::BLUE);
                         debugRenderer->AddBoundingBox({{-0.5, 0}, {0.5, 1}}, Urho3D::Color::BLUE);
                         debugRenderer->AddBoundingBox({{-0.5, 1}, {0.5, 2}}, Urho3D::Color::BLUE);
                         debugRenderer->AddBoundingBox({{-0.5, 2}, {0.5, 3}}, Urho3D::Color::BLUE);
                         debugRenderer->AddBoundingBox({{-0.5, 3}, {0.5, 4}}, Urho3D::Color::BLUE);
                         debugRenderer->AddBoundingBox({{-0.5, 4}, {0.5, 5}}, Urho3D::Color::BLUE);
                         *///this->scene->GetComponent<Urho3D::PhysicsWorld2D>()->DrawDebugGeometry();
                     });
}



void Application::Stop() {}



void Application::update(float) {
    auto uiRoot = GetSubsystem<Urho3D::UI>()->GetRoot();
    auto input = GetSubsystem<Urho3D::Input>();

    Urho3D::String info =
            Urho3D::String {input->GetMousePosition()} + "\n" +
            Urho3D::String {GetSubsystem<Urho3D::Renderer>()->GetViewport(0)->
                            ScreenToWorldPoint(input->GetMousePosition().x_,
                                               input->GetMousePosition().y_, 0)};
    auto text = dynamic_cast<Urho3D::Text*>(
                    uiRoot->GetChild(Urho3D::String{"Text"}));
    text->SetText(info);

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

    auto characterNode = scene->CreateChild("Character");

    cameraNode = characterNode->CreateChild("Camera");
    auto camera = cameraNode->CreateComponent<Urho3D::Camera>();
    camera->SetFarClip(500.0f);
    camera->SetOrthographic(true);
    camera->SetOrthoSize(3);
    //camera->SetOrthoSize((float)GetSubsystem<Urho3D::Graphics>()->GetHeight() * Urho3D::PIXEL_SIZE);
    //camera->SetZoom(std::min((float)GetSubsystem<Urho3D::Graphics>()->GetWidth() / 1280.0f,
    //                         (float)GetSubsystem<Urho3D::Graphics>()->GetHeight() / 800.0f));
    // Set zoom according to user's resolution to ensure full visibility
    // (initial zoom (1.2) is set for full visibility at 1280x800 resolution)

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    Urho3D::SharedPtr<Urho3D::Viewport> viewport
        { new Urho3D::Viewport {GetContext(), scene, camera} };
    GetSubsystem<Urho3D::Renderer>()->SetViewport(0, viewport);

    GetSubsystem<Urho3D::Input>()->SetMouseVisible(true);

}



void Application::readLevelFromJSON(Urho3D::JSONFile* file) {
    auto cache = GetSubsystem<Urho3D::ResourceCache>();
    auto root = file->GetRoot();
    if(scene.Null()) {
        initScene();
    }
    std::vector<Urho3D::Vector2> floorVertices {};
    for(auto& point: root.Get("floor").GetArray()) {
        floorVertices.push_back({point.Get("x").GetFloat(),
                                 point.Get("y").GetFloat()});
    }
    createFloor("Floor", floorVertices);
    for(auto& entity: root.Get("entities").GetArray()) {
        Creature* creature {};
        Urho3D::Node* node {};
        auto type = entity.Get("type").GetString();
        auto filepath = "Objects/Creatures/" + type + ".json";
        auto file = cache->GetResource<Urho3D::JSONFile>(filepath);
        if(type == "PC" ) {
            node = scene->GetChild("Character");
            creature = node->CreateComponent<Character>();

        } else {
            node = scene->CreateChild(type +
                                      file->GetRoot().Get("id").GetString());
            creature = node->CreateComponent<Creature>();
        }
        auto position = entity.Get("position");
        node->Translate2D({position.Get("x").GetFloat(),
                           position.Get("y").GetFloat()});
        creature->initFromJSON(file->GetRoot());
        creature->setName(type);
    }

    auto backgroundRoot = scene->CreateChild("Background");
    for(auto& spriteData: root.Get("background_sprites").GetArray()) {
        auto textureNode = backgroundRoot->
                              CreateChild(spriteData.Get("id").GetString());
        Sprite* sprite =
                textureNode->CreateComponent<Sprite>();
        sprite->initFromJSON(spriteData);
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
    collisionPlane->SetVertices({vertices.data(), static_cast<unsigned>(vertices.size())});
    collisionPlane->SetFriction(1);
    floorBody->AddCollisionShape2D(collisionPlane);
}
