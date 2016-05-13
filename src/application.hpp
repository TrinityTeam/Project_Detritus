#pragma once
#include <Urho3D/Engine/Application.h>
#include <functional>
#include <vector>
namespace Urho3D {
class Node;
class Scene;
class JSONFile;
}



class Application: public Urho3D::Application {
public:
    Application(Urho3D::Context* context);

    virtual void Setup() override;
    virtual void Start() override;
    virtual void Stop() override;

    void update(float timestep);
    void keyDown(int key);

private:
    void initUI();
    void initScene();
    void readLevelFromJSON(Urho3D::JSONFile* file);
    void createCharacter();
    void createFloor(const std::string& name,
                     const std::vector<Urho3D::Vector2>& vertices);


    Urho3D::Node* cameraNode;
    Urho3D::SharedPtr<Urho3D::Scene> scene;
};
