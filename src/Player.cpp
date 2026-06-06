#include "Player.h"
#include <cmath>

Player::Player(float x, float y) : GameObject(x, y), speed(300.0f), health(100) {
    shape.setSize(sf::Vector2f(40.0f, 40.0f));
    shape.setOrigin(20.0f, 20.0f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime) {
    if (isDestroyed()) return;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += speed * deltaTime;

    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

void Player::rotateTowardsMouse(const sf::RenderWindow& window) {
    if (isDestroyed()) return;
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    float angle = std::atan2(dy, dx) * 180.0f / 3.14159265f;
    shape.setRotation(angle);
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        destroy();
    }
}