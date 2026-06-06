#include "Alien.h"
#include <cmath>

Alien::Alien(float x, float y, Player* playerTarget, sf::Texture& tex, float moveSpeed, int maxHp)
    : GameObject(x, y), speed(moveSpeed), target(playerTarget), hp(maxHp) {

    sprite.setTexture(tex);
    sprite.setOrigin(15.0f, 15.0f); // Polowa rozmiaru 30x30
    sprite.setPosition(position);
}

void Alien::update(float deltaTime) {
    if (!target || target->isDestroyed() || isDestroyed()) return;

    // Obliczanie wektora kierunkowego do gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        // Normalizacja i ruch
        float dirX = dx / distance;
        float dirY = dy / distance;

        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;
    }

    sprite.setPosition(position);
}

void Alien::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Alien::getBounds() const {
    return sprite.getGlobalBounds();
}

void Alien::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        destroy();
    }
}