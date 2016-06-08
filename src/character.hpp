#pragma once
#include "creature.hpp"



class Character: public Creature {

    URHO3D_OBJECT(Character, Creature)


public:
    Character(Urho3D::Context* context);

protected:
    void update(float timestep) override;

};
