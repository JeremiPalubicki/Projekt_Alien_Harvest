#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    // Ustawienie punktu centralnego na srodek (opcjonalnie)
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Obstacle::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}