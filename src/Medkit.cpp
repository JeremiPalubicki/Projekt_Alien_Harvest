#include "Medkit.h"

Medkit::Medkit(float startX, float startY, sf::Texture& texture) : GameObject(startX, startY) {
    sprite.setTexture(texture);

    // Pobieramy wymiary oryginalnego obrazka
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Ustawiamy środek ciężkości dokładnie na środek obrazka
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // Skalujemy apteczkę do rozsądnego rozmiaru (np. 30 pikseli)
    float targetSize = 30.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(position);
}

void Medkit::update(float deltaTime) {
  
}

void Medkit::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Medkit::getBounds() const {
    return sprite.getGlobalBounds();
}