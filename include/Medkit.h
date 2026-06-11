#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Medkit : public GameObject {
private:
    sf::Sprite sprite;
public:
    Medkit(float startX, float startY, sf::Texture& texture);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};
