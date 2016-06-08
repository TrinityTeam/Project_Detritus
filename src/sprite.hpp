#pragma once
#include <Urho3D/Urho2D/StaticSprite2D.h>



class Sprite: public Urho3D::StaticSprite2D {

    URHO3D_OBJECT(Sprite, Urho3D::StaticSprite2D)

public:
    Sprite(Urho3D::Context* context);
    void initFromJSON(const Urho3D::JSONValue& data);
    void setSize(const Urho3D::Vector2& size);

private:
    Urho3D::Vector2 size;
};
