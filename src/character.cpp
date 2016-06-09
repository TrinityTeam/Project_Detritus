#include "character.hpp"
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Node.h>
#include "log.hpp"
#include "animation.hpp"



Character::Character(Urho3D::Context* context):
    Creature {context} {

    SubscribeToEvent(Urho3D::E_KEYDOWN,
                     [this](Urho3D::StringHash,
                            Urho3D::VariantMap& eventParams) {
                         auto input = GetSubsystem<Urho3D::Input>();
                         auto key = Urho3D::KeyDown::P_SCANCODE;
                         if(eventParams[key].GetInt() ==
                            input->GetScancodeFromName("Space")) {
                             moveGround({0, 1});
                         }
                     });
    SubscribeToEvent(Urho3D::E_MOUSEBUTTONDOWN,
                     [this](Urho3D::StringHash,
                            Urho3D::VariantMap& eventParams) {
                         auto button = Urho3D::MouseButtonDown::P_BUTTON;
                         if(eventParams[button].GetInt() ==
                            Urho3D::MOUSEB_LEFT) {
                             strike();
                         }
                     });
}



void Character::update(float) {
    auto input = GetSubsystem<Urho3D::Input>();
    auto direction = Urho3D::Vector2::ZERO;

    if(input->GetScancodeDown(input->GetScancodeFromName("A"))) {
        direction.x_ -= 1;
    }
    if(input->GetScancodeDown(input->GetScancodeFromName("D"))) {
        direction.x_ += 1;
    }
    if(isCanActing() and direction.x_ != 0) {
        GetNode()->GetComponent<cAnimation>()
                ->SetFlipX(direction.x_ < 0);
    }

    moveGround(direction);
}
