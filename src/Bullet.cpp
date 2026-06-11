#include "Bullet.h"
#include <cmath> // Do funkcji sin() i cos()

Bullet::Bullet(float x, float y, float angle) : GameObject(x, y) {
    float speed = 700.0f; // Prędkość początkowa pocisku

    // Konwersja kąta ze stopni na radiany (wymagane przez funkcje z biblioteki <cmath>)
    float radians = angle * 3.14159265f / 180.0f;

    // Obliczanie składowych wektora prędkości dla osi X i Y
    velocity.x = std::cos(radians) * speed;
    velocity.y = std::sin(radians) * speed;

    // Konfiguracja wyglądu pocisku (prosty, żółty prostokąt symulujący laser)
    shape.setSize(sf::Vector2f(16.0f, 4.0f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(8.0f, 2.0f); // Ustawienie punktu centralnego

    shape.setPosition(position);
    shape.setRotation(angle); // Obrót grafiki zgodnie z trajektorią lotu
}

void Bullet::update(float deltaTime) {
    if (isDestroyed()) return;

    // Aktualizacja pozycji pocisku na ekranie w oparciu o wektor prędkości
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    shape.setPosition(position);

    // Automatyczne usuwanie pocisku z pamięci, gdy wyleci poza widoczny obszar gry
    // Optymalizuje to zużycie pamięci i wydajność
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