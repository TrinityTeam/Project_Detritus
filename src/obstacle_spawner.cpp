#include "obstacle_spawner.hpp"
#include <type_traits>



ObstacleSpawner::ObstacleSpawner(Urho3D::Context* context):
    Urho3D::LogicComponent {context} {

}



void ObstacleSpawner::start() {
    active = true;
}



void ObstacleSpawner::pause() {
    active = false;
}



void ObstacleSpawner::stop() {
    active = false;
    passedTime = 0;
}



void ObstacleSpawner::Update(float timeStep) {
    if(not active) {
        return;
    }
    passedTime += timeStep;

    auto p = std::end(obstaclesQuery);
    for(p = std::begin(obstaclesQuery);
        p != std::end(obstaclesQuery) and p->first <= passedTime;
        p++) {
            p->second(this);
    }
    obstaclesQuery.erase(std::begin(obstaclesQuery), p);
}
