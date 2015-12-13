#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <list>
#include <memory>
using std::unique_ptr;



class Node: public sf::Drawable, public sf::Transformable {
public:
    Node() = default;
    virtual ~Node() = default;

    void attachChild(unique_ptr<Node> node);
    unique_ptr<Node> detachChild(const Node& node);

private:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override final;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

    void update(sf::Time delta);
    virtual void updateCurrent(sf::Time delta);

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    std::list<unique_ptr<Node>> childrens;
};
