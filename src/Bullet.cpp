#include "Bullet.h"
#include <cmath> // Do funkcji sin() i cos()

Bullet::Bullet(float x, float y, float angle) : GameObject(x, y) {
    float speed = 700.0f; // Szybkość lotu lasera

    // Zamiana stopni na radiany (C++ wymaga radianów w funkcjach trygonometrycznych)
    float radians = angle * 3.14159265f / 180.0f;

    // Obliczanie kierunku lotu pocisku
    velocity.x = std::cos(radians) * speed;
    velocity.y = std::sin(radians) * speed;

    // Tworzenie "świecącego" pocisku
    shape.setSize(sf::Vector2f(16.0f, 4.0f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(8.0f, 2.0f); // Środek pocisku

    shape.setPosition(position);
    shape.setRotation(angle); // Obracamy grafikę pocisku zgodnie z kierunkiem lotu
}

void Bullet::update(float deltaTime) {
    if (isDestroyed()) return;

    // Aktualizacja pozycji na podstawie wektora
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    shape.setPosition(position);

    // Automatyczne niszczenie pocisku, gdy wyleci poza ekran (oszczędza pamięć)
    if (position.x < -100 || position.x > 900 || position.y < -100 || position.y > 700) {
        destroy();
    }
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}