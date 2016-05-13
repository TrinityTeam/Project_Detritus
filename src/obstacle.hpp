#pragma once
#include <Urho3D/Scene/LogicComponent.h>



class Obstacle: public Urho3D::LogicComponent {

    URHO3D_OBJECT(Obstacle, LogicComponent)

public:
    Obstacle(Urho3D::Context* context);

    virtual void Start() override;
    virtual void FixedUpdate(float timestep) override;

    bool isDestroyed() const;

private:
    bool destroyed {false};

};
