#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class AlienBullet : public GameObject {
private:
    sf::CircleShape shape;
    float speed;
    float dirX; // Wektor kierunku na osi X
    float dirY; // Wektor kierunku na osi Y
public:
    AlienBullet(float startX, float startY, sf::Vector2f targetPos);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};
