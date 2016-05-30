#include "character.hpp"
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/IO/Log.h>
#include "obstacle.hpp"



Character::Character(Urho3D::Context* context):
    Urho3D::LogicComponent {context} {

}



void Character::Start() {
    auto cache = GetSubsystem<Urho3D::ResourceCache>();

    auto animationSet = cache->GetResource<Urho3D::AnimationSet2D>("Sprites/Character/character.scml");
    auto sprite = GetNode()->CreateComponent<Urho3D::AnimatedSprite2D>();
    sprite->SetAnimationSet(animationSet);
    sprite->SetAnimation("Stand");
    sprite->SetSpeed(1.5);
    Urho3D::RigidBody2D* body =
            GetNode()->CreateComponent<Urho3D::RigidBody2D>();
    body->SetBodyType(Urho3D::BT_DYNAMIC);
    body->SetMass(1);
    body->SetAllowSleep(false);
    body->SetFixedRotation(true);
    Urho3D::CollisionBox2D* collisionShape =
            GetNode()->CreateComponent<Urho3D::CollisionBox2D>();
    collisionShape->SetFriction(10);
    collisionShape->SetDensity(1);
    collisionShape->SetSize(0.6, 0.6);
    body->AddCollisionShape2D(collisionShape);
}



void Character::FixedUpdate(float) {
    auto body = GetNode()->GetComponent<Urho3D::RigidBody2D>();

    auto input = GetSubsystem<Urho3D::Input>();

    auto direction = Urho3D::Vector2::ZERO;

    if(not isInAir()) {
        if(input->GetScancodeDown(input->GetScancodeFromName("A"))) {
            direction.x_ -= 1;
        }
        if(input->GetScancodeDown(input->GetScancodeFromName("D"))) {
            direction.x_ += 1;
        }
        if(input->GetScancodePress(input->GetScancodeFromName("Space"))) {
            direction.y_ += 1;
        }
        if(direction.x_ != 0) {
            GetNode()->GetComponent<Urho3D::AnimatedSprite2D>()->SetFlipX(direction.x_ < 0);
        }
        if(direction.LengthSquared() > 0.0f) {
            direction.Normalize();
            GetNode()->GetComponent<Urho3D::AnimatedSprite2D>()->SetAnimation("Run");

        } else {
            GetNode()->GetComponent<Urho3D::AnimatedSprite2D>()->SetAnimation("Stand");
        }
    }

    const auto SPEED = Urho3D::Vector2 {3, 6};
    constexpr auto MAX_SPEED = 3.f;
    auto v = body->GetLinearVelocity();
    v += direction*SPEED;
    if(abs(v.x_) > MAX_SPEED) {
        v.x_ = ((v.x_ < 0) ? -1 : 1) * MAX_SPEED;
    }

    body->SetLinearVelocity(v);
}



bool Character::isInAir() const {
    Urho3D::PhysicsRaycastResult2D result;
    Urho3D::PhysicsRaycastResult2D resultLeft;
    Urho3D::PhysicsRaycastResult2D resultRight;
    auto shape = GetNode()->GetComponent<Urho3D::CollisionBox2D>();
    auto world = GetScene()->GetComponent<Urho3D::PhysicsWorld2D>();
    auto position = GetNode()->GetPosition2D();
    auto halfWidth = Urho3D::Vector2 {shape->GetSize().x_/2., 0};
    world->RaycastSingle(result, position,
                         position + world->GetGravity());
    world->RaycastSingle(resultLeft, position + halfWidth,
                         (position + halfWidth) + world->GetGravity());
    world->RaycastSingle(resultRight, position - halfWidth,
                         (position - halfWidth) + world->GetGravity());

    auto halfHeight = shape->GetSize().y_/2.;
    if((result.body_ != nullptr and result.distance_ - halfHeight <= 0.05) or
       (resultLeft.body_ != nullptr and resultLeft.distance_ - halfHeight <= 0.05) or
       (resultRight.body_ != nullptr and resultRight.distance_ - halfHeight <= 0.05)) {
        return false;

    } else {
        return true;
    }

}



Urho3D::Vector2 Character::getLinearVelocity() const {
    return GetNode()->GetComponent<Urho3D::RigidBody2D>()->GetLinearVelocity();
}
