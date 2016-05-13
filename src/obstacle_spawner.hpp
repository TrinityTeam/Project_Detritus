#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/IO/Log.h>
#include <list>
#include <functional>
#include <algorithm>
class Obstacle;




class ObstacleSpawner: public Urho3D::LogicComponent {

    URHO3D_OBJECT(ObstacleSpawner, LogicComponent)

public:
    ObstacleSpawner(Urho3D::Context* context);

    void start();
    void pause();
    void stop();

    template<typename Obstacle_t>
    void addObstacle(float time) {
        static_assert(std::is_base_of<Obstacle, Obstacle_t>(),
                      "ObstacleSpawner can spawn only derived from Obstacle objects");
        static_assert(not std::is_abstract<Obstacle_t>(),
                      "Cannot spaw abstract object");
        auto i = std::upper_bound(std::begin(obstaclesQuery),
                                  std::end(obstaclesQuery),
                                  time,
                         [](const float& p1,
                            const decltype(obstaclesQuery)::value_type& p2) {
                             return p1 < p2.first;
                         });

        obstaclesQuery.insert(i, {time, createObstacle<Obstacle_t>});
    }

    virtual void Update(float timeStep) override;

private:
    template<typename Obstacle_t>
    static void createObstacle(ObstacleSpawner* spawner) {
        auto node = spawner->GetNode()->CreateChild(("Obstacle "+std::to_string(spawner->obstaclesQuery.size())).c_str());
        node->Translate({5, 0});
        node->CreateComponent<Obstacle_t>();
    }

    float passedTime {};
    bool active {false};
    std::list<std::pair<float, std::function<void(ObstacleSpawner*)>>> obstaclesQuery;
};
