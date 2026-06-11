#include "Runner.h"
#include <cmath>

// Wywołujemy konstruktor bazowego Aliena: predkosc 200, hp 1
Runner::Runner(float x, float y, Player* playerTarget, sf::Texture& tex)
    : Alien(x, y, playerTarget, tex, 200.0f, 1), timeAlive(0.0f) {

    // Nadajemy mu czerwonawy odcień, żeby różnił się od zwykłych!
    sprite.setColor(sf::Color(255, 100, 100));
}

void Runner::update(float deltaTime) {
    if (!target || target->isDestroyed() || isDestroyed()) return;

    timeAlive += deltaTime;

    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        // BAZOWY RUCH: przód
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;

        // ZYGZAK: Prostopadle do kierunku lotu
        // Używamy sinusa z czasem, żeby falowało
        float zigzagSize = 80.0f; // Jak mocno ma nim rzucać na boki
        float wave = std::sin(timeAlive * 12.0f) * zigzagSize * deltaTime;

        position.x += -dirY * wave;
        position.y += dirX * wave;
    }

    sprite.setPosition(position);
}