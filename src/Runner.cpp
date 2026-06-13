#include "Runner.h"
#include "Player.h"
#include <cmath>

// Konstruktor Biegacza (Prędkość: 200.0f, HP: 1)
Runner::Runner(float x, float y, Player* playerTarget, sf::Texture& tex)
    : Alien(x, y, playerTarget, tex, 200.0f, 1), timeAlive(0.0f) {

    // Zmiana koloru Biegacza na czerwony, żeby wyróżniał się w tłumie
    sprite.setColor(sf::Color(255, 100, 100));
}

void Runner::update(float deltaTime) {
    // Jeśli nie ma celu lub cel nie żyje, nie rób nic
    if (!target || target->isDestroyed() || isDestroyed()) return;

    timeAlive += deltaTime;

    // Obliczanie wektora kierunkowego do gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        // Standardowy ruch do przodu
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;

        // Mechanika ZYGZAKA (Fale sinusoidalne)
        float zigzagIntensity = 60.0f; // Szerokość uników
        float zigzagSpeed = 15.0f;     // Szybkość uników
        float wave = std::sin(timeAlive * zigzagSpeed) * zigzagIntensity * deltaTime;

        // Dodanie przesunięcia prostopadłego do kierunku ruchu
        position.x += -dirY * wave;
        position.y += dirX * wave;
    }

    sprite.setPosition(position);
}