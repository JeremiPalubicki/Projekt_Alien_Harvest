#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Bullet : public GameObject {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity; // Wektor prędkości w osi X i Y

public:
    Bullet(float x, float y, float angle);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};