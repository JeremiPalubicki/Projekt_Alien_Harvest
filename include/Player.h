#pragma once
#include "GameObject.h"

// klasa gracza, dziedziczy po glownej bazie
class Player : public GameObject {
private:
    sf::RectangleShape shape; // na razie zwykly prostokat zeby bylo cokolwiek widac
    float speed;              // predkosc w pikselach na sekunde

public:
    Player(float x, float y);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    // funkcja do obracania postacia w strone celownika
    void rotateTowardsMouse(const sf::RenderWindow& window);
};