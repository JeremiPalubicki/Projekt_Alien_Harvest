#include "AlienBullet.h"
#include <cmath> // Potrzebne do obliczenia odległości

AlienBullet::AlienBullet(float startX, float startY, sf::Vector2f targetPos)
    : GameObject(startX, startY), speed(600.0f) {

    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(5.0f, 5.0f);
    shape.setPosition(position);

    // --- CELOWANIE (Pitagoras) ---
    // Obliczamy różnicę pozycji między graczem a lufą kosmity
    float dx = targetPos.x - startX;
    float dy = targetPos.y - startY;

    // Odległość całkowita
    float distance = std::sqrt(dx * dx + dy * dy);

    // Normalizacja wektora (żeby pocisk zawsze leciał z tą samą prędkością, niezależnie od odległości gracza)
    if (distance > 0) {
        dirX = dx / distance;
        dirY = dy / distance;
    }
    else {
        dirX = -1.0f; // Zabezpieczenie: jeśli coś pójdzie nie tak, strzel prosto w lewo
        dirY = 0.0f;
    }
}

void AlienBullet::update(float deltaTime) {
    // Lot po skosie dokładnie tam, gdzie stał gracz w momencie strzału
    position.x += dirX * speed * deltaTime;
    position.y += dirY * speed * deltaTime;
    shape.setPosition(position);

    // Niszczymy pocisk, gdy wyleci poza którąkolwiek z krawędzi ekranu
    if (position.x < -50.0f || position.x > 850.0f || position.y < -50.0f || position.y > 650.0f) {
        destroy();
    }
}

void AlienBullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect AlienBullet::getBounds() const {
    return shape.getGlobalBounds();
}