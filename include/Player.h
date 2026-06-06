#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Player : public GameObject {
private:
    sf::RectangleShape shape;
    float speed;
    int health; // Punkty zycia farmera

public:
    Player(float x, float y);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void rotateTowardsMouse(const sf::RenderWindow& window);
    
    // Metody do HP
    void takeDamage(int amount);
    int getHealth() const { return health; }
}; // <--- To jest ta najwazniejsza klamra ze srednikiem!