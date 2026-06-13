#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, const sf::Texture& texture) {
    sprite.setTexture(texture);

    // Pobranie oryginalnych wymiarów tekstury drzewa
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Ustawienie punktu centralnego dokładnie na środek
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    // Skalowanie drzewa do rozmiaru postaci gracza (60 pikseli)
    float targetSize = 60.0f;

    // Opcjonalnie: Jeśli chcesz, żeby drzewo było ciut większe od gracza 
    // (żeby lepiej wyglądało), zmień targetSize np. na 80.0f lub 100.0f

    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    sprite.setPosition(x, y);
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Obstacle::getGlobalBounds() const {
    // Odrobina magii: zmniejszamy "hitbox" o 20%, żeby kolizja 
    // była tylko na pniu, a nie na krawędziach liści!
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return sf::FloatRect(
        bounds.left + bounds.width * 0.2f,
        bounds.top + bounds.height * 0.2f,
        bounds.width * 0.6f,
        bounds.height * 0.6f
    );
}