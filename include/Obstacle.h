#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite sprite;
public:
    Obstacle(float x, float y, const sf::Texture& texture);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getGlobalBounds() const;
};#pragma once
