#pragma once
#include <Urho3D/Scene/LogicComponent.h>
namespace Urho3D {
class RigidBody2D;
class PhysicsWorld2D;
}



class Character: public Urho3D::LogicComponent {

    URHO3D_OBJECT(Character, LogicComponent)

public:
    Character(Urho3D::Context* context);

    virtual void Start() override;
    virtual void FixedUpdate(float timestep) override;

    bool isInAir() const;

    Urho3D::Vector2 getLinearVelocity() const;

private:
};
