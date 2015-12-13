#include "node.hpp"



void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    drawCurrent(target, states);
    for(auto& child: childrens) {
        child->drawCurrent(target, states);
    }
}



void Node::drawCurrent(sf::RenderTarget& target,
                       sf::RenderStates states) const {

}



void Node::update(sf::Time delta) {
    updateCurrent(delta);
    for(auto& child: childrens) {
        child->updateCurrent(delta);
    }
}



void Node::updateCurrent(sf::Time delta) {

}
