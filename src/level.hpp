#pragma once
#include <string>
#include <memory>
#include <vector>
namespace Urho3D {
class Vector2;
class Node;
}
class Character;



class Level {
public:
    Level();

    Urho3D::Node* loadJSON(const std::string& filepath);

private:
    Urho3D::Node* createFloor(const std::string& name,
                              const std::vector<Urho3D::Vector2>& vertices);

};
