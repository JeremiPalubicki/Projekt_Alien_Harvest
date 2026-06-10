#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Player : public GameObject {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    int health;
    int maxHealth;
    float aimAngle;
public:
    Player(float x, float y);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void rotateTowardsMouse(const sf::RenderWindow& window);

    void takeDamage(int amount);
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

 
    float getRotation() const { return aimAngle; }
};