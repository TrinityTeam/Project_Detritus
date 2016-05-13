#include "obstacle.hpp"
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/IO/Log.h>



Obstacle::Obstacle(Urho3D::Context* context):
    Urho3D::LogicComponent {context} {
}



void Obstacle::Start() {
    auto cache = GetSubsystem<Urho3D::ResourceCache>();

    auto sprite = GetNode()->CreateComponent<Urho3D::StaticSprite2D>();
    sprite->SetSprite(cache->GetResource<Urho3D::Sprite2D>("Urho2D/Box.png"));
    Urho3D::RigidBody2D* body = GetNode()->CreateComponent<Urho3D::RigidBody2D>();
    body->SetBodyType(Urho3D::BT_KINEMATIC);
    Urho3D::CollisionBox2D* collisionBox = GetNode()->CreateComponent<Urho3D::CollisionBox2D>();
    collisionBox->SetFriction(0);
    collisionBox->SetSize({0.3, 0.3});
    body->AddCollisionShape2D(collisionBox);

    body->SetLinearVelocity({-3, 0});
}



void Obstacle::FixedUpdate(float timestep) {
    if(GetNode()->GetPosition().x_ < -5) {
        GetNode()->RemoveComponent<Urho3D::StaticSprite2D>();
        GetNode()->RemoveComponent<Urho3D::RigidBody2D>();
        GetNode()->RemoveComponent<Urho3D::CollisionBox2D>();
        destroyed = true;
    }
}
