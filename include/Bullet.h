#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    sf::RectangleShape shape;
    float speed;
    sf::Vector2f velocity;

public:
    Bullet(float x, float y, float angle);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};