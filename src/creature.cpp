#include "creature.hpp"
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Urho2D/SpriterData2D.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/Font.h>
#include "animation.hpp"
#include "log.hpp"



Creature::Creature(Urho3D::Context* context):
    Urho3D::LogicComponent {context} {

}



void Creature::Start() {
    GetNode()->CreateComponent<cAnimation>();
    GetNode()->CreateComponent<Urho3D::RigidBody2D>();
    GetNode()->CreateComponent<Urho3D::CollisionBox2D>();
    auto databar = GetNode()->CreateChild("Databar");
    Urho3D::Text3D* text = databar->CreateComponent<Urho3D::Text3D>();
    text->SetText("Someone");
    auto cache = GetSubsystem<Urho3D::ResourceCache>();
    text->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Anonymous Pro.ttf"), 8);
    text->SetColor(Urho3D::Color::RED);
    text->SetEffectColor({0.5f, 0.5f, 0.5f});
    text->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);
}



void Creature::FixedUpdate(float timestep) {
    update(timestep);

    auto animation = GetNode()->GetComponent<cAnimation>();

    if(animation->isOver() and not animation->isLooping()) {
        setState(pendingState);
    }
    auto body = GetNode()->GetComponent<Urho3D::RigidBody2D>();

    if(state == State::Hover) {
        if(not isInAir(Part::Center)) {
            setState(pendingState);

        } else {
            if(not isInAir(Part::Left)) {
                moveAir({1, 0}, 0.6);
            } else if(not isInAir(Part::Right)) {
                moveAir({-1, 0}, 0.6);
            }
        }
    } else if(isCanActing()){
        if(isInAir(Part::Center)) {
           setState(State::Hover);

        } else if(body->GetLinearVelocity().x_ == 0) {
            setState(State::Idle);

        } else {
            setState(State::Run);
        }
    }
}



void Creature::initFromJSON(const Urho3D::JSONValue& data) {
    auto cache = GetSubsystem<Urho3D::ResourceCache>();

    auto animationSet = cache->GetResource<Urho3D::AnimationSet2D>(
                            data.Get("spritesheet").GetString());
    auto sprite = GetNode()->GetComponent<cAnimation>();
    sprite->SetAnimationSet(animationSet);
    sprite->SetAnimation("Stand");
    sprite->SetSpeed(data.Get("animation_speed").GetInt());
    sprite->SetLayer(data.Get("layer").GetInt());

    Urho3D::RigidBody2D* body =
            GetNode()->GetComponent<Urho3D::RigidBody2D>();
    body->SetBodyType(Urho3D::BT_DYNAMIC);
    body->SetMass(1);
    body->SetAllowSleep(false);
    body->SetFixedRotation(true);
    Urho3D::CollisionBox2D* collisionShape =
            GetNode()->GetComponent<Urho3D::CollisionBox2D>();
    collisionShape->SetFriction(data.Get("friction").GetFloat());
    collisionShape->SetDensity(data.Get("density").GetFloat());
    collisionShape->SetSize(data.Get("size").Get("width").GetFloat(),
                            data.Get("size").Get("height").GetFloat());
    body->AddCollisionShape2D(collisionShape);
    GetNode()->GetChild("Databar")
            ->Translate2D({0, collisionShape->GetSize().y_/1});

}



void Creature::setName(const Urho3D::String& name) {
    auto databar = GetNode()->GetChild("Databar")->GetComponent<Urho3D::Text3D>();
    databar->SetText(name);
}



bool Creature::isInAir(Part::Side side_mask) const {
    auto shape = GetNode()->GetComponent<Urho3D::CollisionBox2D>();
    auto halfWidth = Urho3D::Vector2 {shape->GetSize().x_/2.f, 0};
    if(((side_mask & Part::Center) and checkGroundBeneath({0, 0})) or
       ((side_mask & Part::Right) and checkGroundBeneath(halfWidth)) or
       ((side_mask & Part::Left) and checkGroundBeneath(-halfWidth))) {
        return false;

    } else {
        return true;
    }
}



bool Creature::isCanActing() const {
    return canActing;
}



void Creature::moveGround(Urho3D::Vector2 dir, float speedMultiplier) {
    if(isCanActing()) {
        move(dir, speedMultiplier);
    }
}



void Creature::moveAir(Urho3D::Vector2 dir, float speedMultiplier) {
    if(state == State::Hover) {
        move(dir, speedMultiplier);
    }
}



void Creature::move(Urho3D::Vector2 dir, float speedMultiplier) {
    moveWithoutMaxSpeedLimitation(dir, speedMultiplier);
    auto body = GetNode()->GetComponent<Urho3D::RigidBody2D>();
    auto v = body->GetLinearVelocity();
    if(fabs(v.x_) > maxSpeed) {
        v.x_ = ((v.x_ < 0) ? -1 : 1) * maxSpeed;
    }
    body->GetBody()->SetLinearVelocity({v.x_, v.y_});
}



void Creature::moveWithoutMaxSpeedLimitation(Urho3D::Vector2 dir,
                                             float speedMultiplier) {
    if(dir.LengthSquared() > 0) {
        dir.Normalize();
    }
    auto body = GetNode()->GetComponent<Urho3D::RigidBody2D>();
    auto v = body->GetLinearVelocity();
    v += dir * acceleration * speedMultiplier;
    body->GetBody()->SetLinearVelocity({v.x_, v.y_});
}



void Creature::strike() {
    if(not isCanActing()) return;
    setState(State::Strike);
    auto world = GetScene()->GetComponent<Urho3D::PhysicsWorld2D>();
    Urho3D::PhysicsRaycastResult2D result;
    auto sprite = GetNode()->GetComponent<cAnimation>();
    world->RaycastSingle(result, GetNode()->GetPosition2D(),
                         GetNode()->GetPosition2D() +
                         (Urho3D::Vector2::RIGHT * (sprite->GetFlipX()?-1:1)));
    if(result.body_ == nullptr) return;
    auto targetNode = result.body_->GetNode();

    if(targetNode->HasComponent<Creature>()) {
        targetNode->GetComponent<Creature>()->recieveDamage(this);
    }
}



void Creature::recieveDamage(const Creature* attacker) {
    moveWithoutMaxSpeedLimitation(GetNode()->GetPosition2D() -
                                  attacker->GetNode()->GetPosition2D(), 5);
    setState(State::Decay);
}



void Creature::setState(State state) {
    if(this->state == state) return;
    auto sprite = GetNode()->GetComponent<cAnimation>();

    switch(state) {
    case State::Decay:
        sprite->SetAnimation("Death", LM_FORCE_CLAMPED);
        pendingState = state;
        canActing = false;
        break;

    case State::Idle:
        sprite->SetAnimation("Stand");
        canActing = true;
        break;

    case State::Hover:
        sprite->SetAnimation("Run", LM_FORCE_CLAMPED);
        pendingState = this->state;
        canActing = false;
        break;

    case State::Run:
        sprite->SetAnimation("Run");
        canActing = true;
        break;

    case State::Strike:
        sprite->SetAnimation("Attack", LM_FORCE_CLAMPED);
        pendingState = this->state;
        canActing = false;
        break;

    default:
        ;
    }
    this->state = state;
}



bool Creature::checkGroundBeneath(Urho3D::Vector2 start) const {
    auto world = GetScene()->GetComponent<Urho3D::PhysicsWorld2D>();
    start = GetNode()->GetPosition2D() + start;
    Urho3D::PhysicsRaycastResult2D result;
    world->RaycastSingle(result, start,
                         start + world->GetGravity());
    auto shape = GetNode()->GetComponent<Urho3D::CollisionBox2D>();
    auto halfHeight = shape->GetSize().y_/2.;
    if(result.body_ != nullptr and result.distance_ - halfHeight <= 0.05) {
        return true;

    } else {
        return false;
    }
}
