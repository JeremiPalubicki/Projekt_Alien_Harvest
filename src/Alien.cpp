#include "Alien.h"
#include <cmath>

Alien::Alien(float x, float y, Player* playerTarget) : GameObject(x, y), speed(100.0f), target(playerTarget) {
    // czerwony kwadrat jako tymczasowy kosmita
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setOrigin(15.0f, 15.0f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Red);
}

void Alien::update(float deltaTime) {
    // jak gracza nie ma to stoi w miejscu
    if (!target || target->isDestroyed()) return;

    // liczenie wektora do gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    
    float distance = std::sqrt(dx * dx + dy * dy);
    
    // zeby nie wibrowal z bliska
    if (distance > 0) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;
    }
    
    shape.setPosition(position);
    
    // tutaj bedzie trzeba kiedys sprawdzic czy oberwal z lasera
}

void Alien::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Alien::getBounds() const {
    return shape.getGlobalBounds();
}