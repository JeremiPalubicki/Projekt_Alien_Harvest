#pragma once
#include <SFML/Graphics.hpp>

// glowna klasa bazowa dla obiektow w grze
class GameObject {
protected: 
    sf::Vector2f position; 
    float rotation;        // rotacja bedzie w stopniach/s
    bool destroyed;        // flaga zeby latwo usuwac martwe obiekty z wektora

public:
    GameObject(float x, float y) : position(x, y), rotation(0.0f), destroyed(false) {}
    virtual ~GameObject() = default;

    // metody czysto wirtualne - kazdy typ (gracz/wrog) musi to nadpisac pod siebie
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0; // hitbox pod kolizje

    bool isDestroyed() const { return destroyed; }
    void destroy() { destroyed = true; }
    sf::Vector2f getPosition() const { return position; }
};