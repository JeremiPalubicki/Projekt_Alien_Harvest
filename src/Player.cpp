#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(float x, float y) : GameObject(x, y), speed(300.0f), health(100), maxHealth(100) {
    // wywali blad w konsoli jak zapomnimy dorzucic folderu assets
    if (!texture.loadFromFile("assets/farmer.png")) {
        std::cerr << "Blad krytyczny: nie znaleziono pliku assets/farmer.png!" << std::endl;
    }
    sprite.setTexture(texture);

    // pobieramy oryginalne wymiary, bo grafika pobrana z neta jest za wielka
    sf::FloatRect bounds = sprite.getLocalBounds();

    // ustawiamy srodek obrotu na srodek brzucha (zeby nie krecil sie wokol lewego rogu)
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // skalujemy chlopka w dol, zeby mial sensowne 60 pikseli na mapie
    float targetSize = 60.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(position);
}

void Player::update(float deltaTime) {
    if (isDestroyed()) return;

    // standardowe chodzenie na WSADzie
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += speed * deltaTime;

    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    if (!isDestroyed()) {
        window.draw(sprite);
    }
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds(); // potrzebne do sprawdzania kolizji w mainie
}

void Player::rotateTowardsMouse(const sf::RenderWindow& window) {
    if (isDestroyed()) return;

    // trygonometria do obracania postaci za kursorem
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