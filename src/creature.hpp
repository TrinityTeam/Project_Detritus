#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <map>
#include <chrono>



class Creature: public Urho3D::LogicComponent {

    URHO3D_OBJECT(Creature, LogicComponent)

public:
    enum class State: uint8_t {Idle = 0, Strike, Hover, Run, Decay, Bleed};

    struct Part { enum Side {Right = 0b001, Center = 0b010, Left = 0b100}; };

public:
    Creature(Urho3D::Context* context);

    void Start() override;
    void FixedUpdate(float timestep) override final;

    void initFromJSON(const Urho3D::JSONValue& data);

    bool isInAir(Part::Side side_mask) const;
    bool isCanActing() const;

    void setName(const Urho3D::String& name);
    void moveGround(Urho3D::Vector2 dir, float speedMultiplier = 1);
    void moveAir(Urho3D::Vector2 dir, float speedMultiplier = 1);
    void strike();
    void recieveDamage(const Creature* attacker);

protected:
    virtual void update(float) {}
    void setState(State state);

private:
    void move(Urho3D::Vector2 dir, float speedMultiplier = 1);
    void moveWithoutMaxSpeedLimitation(Urho3D::Vector2 dir,
                                       float speedMultiplier = 1);
    bool checkGroundBeneath(Urho3D::Vector2 start) const; // related to creature node
    void resumeStrike();

    Urho3D::Vector2 acceleration {1, 8};
    std::chrono::milliseconds strikeDelay {460};
    const float meleeAttackFoV {120};
    const float meleeAttackRange {0.4};
    bool striked {false};
    State state {State::Idle};
    State pendingState {State::Idle};
    float maxSpeed {3.f};
    bool canActing {true};
};

