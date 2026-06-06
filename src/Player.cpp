#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(float x, float y) : GameObject(x, y), speed(300.0f), health(100), maxHealth(100) {
    // Wczytywanie tekstury z zabezpieczeniem
    if (!texture.loadFromFile("assets/farmer.png")) {
        std::cerr << "Blad krytyczny: nie znaleziono pliku assets/farmer.png!" << std::endl;
    }
    sprite.setTexture(texture);

    // Ustawienie punktu centralnego sprite'a (zakladamy rozmiar 40x40)
    sprite.setOrigin(20.0f, 20.0f);
    sprite.setPosition(position);
}

void Player::update(float deltaTime) {
    if (isDestroyed()) return;

    // P³ynny ruch
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += speed * deltaTime;

    // Aktualizacja pozycji grafiki na ekranie
    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    if (!isDestroyed()) {
        window.draw(sprite);
    }
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::rotateTowardsMouse(const sf::RenderWindow& window) {
    if (isDestroyed()) return;

    // Trygonometria do obracania bohatera w strone kursora
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    float angle = std::atan2(dy, dx) * 180.0f / 3.14159265f;

    sprite.setRotation(angle);
}

void Player::takeDamage(int amount) {
    health -= amount;
    std::cout << "Otrzymano obrazenia! Aktualne HP: " << health << "/" << maxHealth << std::endl;
    if (health <= 0) {
        destroy();
        std::cout << "Koniec gry! Farmer polegl." << std::endl;
    }
}