#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "node.hpp"


class Sprite: public Node {
public:
    explicit Sprite(const sf::Texture& texture);

    void setTextureRect(const sf::IntRect& rect);

private:
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const override;

    sf::Sprite sprite;
};
