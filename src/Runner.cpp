#include "Runner.h"
#include <cmath>

// Konstruktor klasy Runner wywołuje konstruktor bazowy Alien: wyższa prędkość (200.0f) i mniej zdrowia (1 HP)
Runner::Runner(float x, float y, Player* playerTarget, sf::Texture& tex)
    : Alien(x, y, playerTarget, tex, 200.0f, 1), timeAlive(0.0f) {

    // Nałożenie czerwonego filtru na teksturę, by wizualnie zasugerować graczowi, że wróg jest szybszy
    sprite.setColor(sf::Color(255, 100, 100));
}

void Runner::update(float deltaTime) {
    if (!target || target->isDestroyed() || isDestroyed()) return;

    // Aktualizacja czasu życia obiektu (wymagane do efektu falowania)
    timeAlive += deltaTime;

    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        // BAZOWY RUCH: standardowe poruszanie się w kierunku gracza
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;

        // MODYFIKATOR RUCHU (Zygzak): dodanie ruchu prostopadłego do głównej trajektorii
        float zigzagSize = 80.0f; // Amplituda fali (jak szeroko rzuca przeciwnikiem na boki)
        // Użycie funkcji sinus z czasem do stworzenia płynnego, oscylującego ruchu
        float wave = std::sin(timeAlive * 12.0f) * zigzagSize * deltaTime;

        // Aplikacja fali na osie prostopadłe (-dirY, dirX)
        position.x += -dirY * wave;
        position.y += dirX * wave;
    }

    sprite.setPosition(position);
}