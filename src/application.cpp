#include "application.hpp"
#include <SFML/Window/Event.hpp>



Application::Application():
    window(sf::VideoMode(1024, 768), "Game"),
    player(playerTexture) {
    textures.load("character", "../data/textures/character.png");
    playerTexture = textures.get("character");
    player.setTextureRect(sf::IntRect(sf::Vector2i{0, 0},
                                      sf::Vector2i { playerTexture.getSize()}));
}



void Application::run() {
    sf::Clock clock;
    sf::Time sinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        processEvents();

        sinceLastUpdate += clock.restart();
        while(sinceLastUpdate > TIME_PER_FRAME) {
            sinceLastUpdate -= TIME_PER_FRAME;
            processEvents();
            update(TIME_PER_FRAME);
        }
        render();
    }
}



void Application::render() {
    window.clear(sf::Color::Green);

    window.draw(player);

    window.display();
}



void Application::update(sf::Time delta) {

}



void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            if(event.key.code == sf::Keyboard::Escape) {
                window.close();
                break;
            }
        default:
            ;
        }
    }
}
