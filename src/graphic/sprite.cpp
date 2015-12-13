#include "sprite.hpp"
#include <SFML/Graphics/RenderTarget.hpp>



Sprite::Sprite(const sf::Texture& texture):
    sprite(texture) {

}



void Sprite::setTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
}



void Sprite::drawCurrent(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    target.draw(sprite, states);
}
