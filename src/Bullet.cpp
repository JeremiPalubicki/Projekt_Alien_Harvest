#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float angle) : GameObject(x, y), speed(500.0f) {
    rotation = angle;

    shape.setSize(sf::Vector2f(10.0f, 4.0f));
    shape.setOrigin(5.0f, 2.0f);
    shape.setPosition(position);
    shape.setRotation(rotation);
    shape.setFillColor(sf::Color::Yellow);

    // Przeliczenie kąta na wektor kierunku (radiany)
    float rad = rotation * 3.14159265f / 180.0f;
    velocity.x = std::cos(rad);
    velocity.y = std::sin(rad);
}

void Bullet::update(float deltaTime) {
    position += velocity * speed * deltaTime;
    shape.setPosition(position);

    // Usuwanie pocisku poza ekranem (okno 800x600)
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600) {
        destroy();
    }
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}