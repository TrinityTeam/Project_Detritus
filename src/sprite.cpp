#include "sprite.hpp"
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/Sprite2D.h>


Sprite::Sprite(Urho3D::Context* context):
    Urho3D::StaticSprite2D {context} {

}



void Sprite::setSize(const Urho3D::Vector2& size) {
    auto vertices = GetSourceBatches()[0].vertices_;
    const Urho3D::Rect rectangle { vertices[0].position_.x_, vertices[1].position_.y_,
                                   vertices[2].position_.x_, vertices[3].position_.y_ };
    this->size = { static_cast<float>(size.x_ / fabs(rectangle.Size().x_)),
                   static_cast<float>(size.y_ / fabs(rectangle.Size().y_)) };
    if(GetNode()) {
        GetNode()->SetScale2D(this->size);
    }
}



void Sprite::initFromJSON(const Urho3D::JSONValue& data) {
    auto sprite = GetSubsystem<Urho3D::ResourceCache>()->GetResource
                  <Urho3D::Sprite2D>("Sprites/LevelBackgrounds/"+
                                     data.Get("file").GetString());

    SetSprite(sprite);
    SetLayer(data.Get("layer").GetInt());

    auto size = data.Get("size");
    setSize({size.Get("x").GetFloat(), size.Get("y").GetFloat()});
    auto position = data.Get("position");
    if(GetNode()) {
        GetNode()->SetPosition2D({position.Get("x").GetFloat(),
                                  position.Get("y").GetFloat()});
    }
}
