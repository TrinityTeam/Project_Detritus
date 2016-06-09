#pragma once
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <chrono>



class cAnimation: public Urho3D::AnimatedSprite2D {

    URHO3D_OBJECT(cAnimation, Urho3D::AnimatedSprite2D)

public:
    cAnimation(Urho3D::Context* context);

    bool isOver() const;
    bool isLooping() const;
    std::chrono::milliseconds getCurrentTime() const;

};
