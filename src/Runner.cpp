#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(float x, float y) : GameObject(x, y), speed(300.0f), health(100), maxHealth(100) {
    // Wczytanie tekstury - błąd wyświetli się w konsoli, gdy brakuje pliku
    if (!texture.loadFromFile("assets/farmer.png")) {
        std::cerr << "Blad krytyczny: nie znaleziono pliku assets/farmer.png!" << std::endl;
    }
    sprite.setTexture(texture);

    // Pobranie oryginalnych wymiarów tekstury dla poprawnego skalowania i centrowania
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Ustawienie punktu obrotu (Origin) dokładnie na środek sprite'a
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // Skalowanie postaci, aby dopasować ją do rozmiaru świata gry (60 pikseli)
    float targetSize = 60.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(position);
}

void Player::update(float deltaTime) {
    if (isDestroyed()) return;

    // Obsługa ruchu gracza za pomocą klawiszy WASD
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
    return sprite.getGlobalBounds(); // Używane do wykrywania kolizji z przeciwnikami
}

void Player::rotateTowardsMouse(const sf::RenderWindow& window) {
    if (isDestroyed()) return;

    // NAPRAWA MILESTONE 8: Używamy mapPixelToCoords, żeby celowanie działało z przesuwaną kamerą!
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Ponowne przeliczenie współczynników skali, aby zapobiec deformacjom przy zmianie zwrotu postaci
    float targetSize = 60.0f;
    float scaleX = targetSize / bounds.width;
    float scaleY = targetSize / bounds.height;

    // --- OBLICZANIE KĄTA CELOWANIA DLA POCISKÓW ---
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    // std::atan2 zwraca kąt w radianach, zamieniamy go na stopnie do wykorzystania w klasie Bullet
    aimAngle = std::atan2(dy, dx) * 180.0f / 3.14159265f;

    // Mechanizm odbicia lustrzanego grafiki (aby postać patrzyła w stronę kursora)
    if (mousePos.x < position.x) {
        sprite.setScale(scaleX, scaleY);   // Kursor po lewej: standardowa orientacja (zakładając, że grafika wyjściowa patrzy w lewo)
    }
    else {
        sprite.setScale(-scaleX, scaleY);  // Kursor po prawej: odbicie lustrzane osi X
    }

    // Zamrażamy obrót wizualny postaci, aby uniknąć efektu "chodzenia do góry nogami"
    sprite.setRotation(0.0f);
}

void Player::takeDamage(int amount) {
    health -= amount;
    std::cout << "Otrzymano obrazenia! Aktualne HP: " << health << "/" << maxHealth << std::endl;

    // Sprawdzenie stanu śmierci
    if (health <= 0) {
        destroy();
        std::cout << "Koniec gry! Farmer polegl." << std::endl;
    }
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) {
        health = maxHealth; // Zabezpieczenie, żeby nie przekroczyć 100 punktów
    }
}