#include "Alien.h"
#include <cmath>

Alien::Alien(float x, float y, Player* playerTarget, sf::Texture& tex, float moveSpeed, int maxHp)
    : GameObject(x, y), speed(moveSpeed), target(playerTarget), hp(maxHp) {

    sprite.setTexture(tex);

    // to samo co u gracza, ogarniamy srodek do obrotu
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // kosmita musi byc minimalnie mniejszy od farmera zeby dalo sie uciekac
    float targetSize = 50.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(position);
}

void Alien::update(float deltaTime) {
    // jak nie ma celu albo gracz nie zyje, to ufoludki stoja w miejscu
    if (!target || target->isDestroyed() || isDestroyed()) return;

    // wektor kierunkowy na gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;

    // pitagoras do dystansu
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        // normalizacja wektora zeby nie zapierdzielal szybciej chodzac na ukos
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
        destroy(); // dajemy flage zeby wywalilo go z wektora w main.cpp
    }
}