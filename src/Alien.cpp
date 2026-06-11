#include "Alien.h"
#include <cmath>

Alien::Alien(float x, float y, Player* playerTarget, sf::Texture& tex, float moveSpeed, int maxHp)
    : GameObject(x, y), speed(moveSpeed), target(playerTarget), hp(maxHp) {

    sprite.setTexture(tex);

    // Ustawienie środka obrotu i pozycji sprite'a na jego geometryczny środek
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // Skalowanie kosmity do docelowego rozmiaru (np. 50 pikseli), aby był odpowiednio mniejszy od gracza
    float targetSize = 50.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(position);
}

void Alien::update(float deltaTime) {
    // Przerwanie aktualizacji, jeśli brakuje celu, gracz nie żyje lub kosmita został zniszczony
    if (!target || target->isDestroyed() || isDestroyed()) return;

    // Obliczanie wektora kierunkowego wskazującego na pozycję gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;

    // Obliczanie odległości między kosmitą a graczem (twierdzenie Pitagorasa)
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        // Normalizacja wektora kierunkowego - zapobiega to szybszemu poruszaniu się po skosie
        float dirX = dx / distance;
        float dirY = dy / distance;

        // Aktualizacja pozycji kosmity na podstawie wektora, prędkości i czasu klatki (deltaTime)
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
    // Jeśli punkty życia spadną do zera lub poniżej, oznacz obiekt jako zniszczony
    if (hp <= 0) {
        destroy(); 
    }
}