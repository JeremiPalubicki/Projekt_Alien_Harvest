#pragma once
#include "GameObject.h"
#include "Player.h"

class Alien : public GameObject {
private:
    sf::RectangleShape shape;
    float speed;
    Player* target;
    int hp; // Punkty zycia ufoludka

public:
    Alien(float x, float y, Player* playerTarget);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    void takeDamage(int amount);
};