#pragma once
#include "GameObject.h"
#include "Player.h" // zeby kosmita wiedzial gdzie jest gracz

// klasa obcego do gonienia gracza
class Alien : public GameObject {
private:
    sf::RectangleShape shape;
    float speed;
    Player* target; // wskaznik na cel
    
    //  Zmienna przechowujaca zycie kosmity
    int hp; 

public:
    Alien(float x, float y, Player* playerTarget);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    // Deklaracja metody do otrzymywania obrazen
    void takeDamage(int amount); 
};