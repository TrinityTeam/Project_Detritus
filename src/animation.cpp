#include "animation.hpp"
#include <Urho3D/Urho2D/SpriterInstance2D.h>



cAnimation::cAnimation(Urho3D::Context* context):
    Urho3D::AnimatedSprite2D {context} {

}



std::chrono::milliseconds cAnimation::getCurrentTime() const {
    using namespace std::chrono;
    return milliseconds {static_cast<int64_t>(spriterInstance_->GetCurrentTime()*1000)};
}



bool cAnimation::isOver() const {
    return spriterInstance_->GetCurrentTime() >= spriterInstance_->GetAnimation()->length_;
}



bool cAnimation::isLooping() const {
    switch(GetLoopMode()) {
    case LM_FORCE_CLAMPED:
        return false;

    case LM_FORCE_LOOPED:
        return true;

    case LM_DEFAULT:
        return spriterInstance_->GetAnimation()->looping_;
    }
    return true;  // to avoid compiler warning
}
