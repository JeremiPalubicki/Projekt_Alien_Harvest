#pragma once
#include "GameObject.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

class Alien : public GameObject {
protected:
    sf::Sprite sprite;
    float speed;
    Player* target;
    int hp;

public:
    // Konstruktor przyjmuje teraz dodatkowo predkosc i punkty zycia
    Alien(float x, float y, Player* playerTarget, sf::Texture& tex, float moveSpeed = 100.0f, int maxHp = 2);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    void takeDamage(int amount);
};