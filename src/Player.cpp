#include "Player.h"
#include <cmath>

//  inicjalizacje health(100) na liscie inicjalizacyjnej
Player::Player(float x, float y) : GameObject(x, y), speed(300.0f), health(100) {
    // tymczasowy wyglad gracza
    shape.setSize(sf::Vector2f(40.0f, 40.0f));
    shape.setOrigin(20.0f, 20.0f); // srodek rotacji
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Player::update(float deltaTime) {
    // ruch wsad z uwzglednieniem czasu klatki zeby plynnie chodzilo
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.0f;

    // normalizacja wektora zeby po skosie nie biegal szybciej
    if (movement.x != 0.0f || movement.y != 0.0f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x /= length;
        movement.y /= length;
    }

    position += movement * speed * deltaTime;
    shape.setPosition(position);
    shape.setRotation(rotation);
}

void Player::rotateTowardsMouse(const sf::RenderWindow& window) {
    // ogarnianie pozycji myszki wzgledem okna
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    float dx = mousePos.x - position.x;
    float dy = mousePos.y - position.y;
    
    // zamiana radiany na stopnie
    rotation = (std::atan2(dy, dx)) * 180.0f / 3.14159265f;
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

// Implementacja funkcji otrzymywania obrazen przez gracza
void Player::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        destroy(); // Jeśli HP spadnie do 0, farmer odpala flagę zniszczenia z GameObject
    }
}