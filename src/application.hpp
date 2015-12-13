#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "graphic/sprite.hpp"
#include "resource_manager/resource_manager.hpp"



class Application {
public:
    Application();

    void run();

private:
    void render();
    void update(sf::Time delta);
    void processEvents();

    ResourceManager<std::string, sf::Texture> textures;
    sf::RenderWindow window;
    sf::Texture playerTexture;
    Sprite player;

    const sf::Time TIME_PER_FRAME { sf::seconds(1./60.) };
};
