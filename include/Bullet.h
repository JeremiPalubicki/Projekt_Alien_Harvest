#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

/// Klasa reprezentująca pocisk wystrzeliwany w grze.
class Bullet : public GameObject {
private:
    sf::RectangleShape shape;   // Kształt pocisku (prosty prostokąt)
    sf::Vector2f velocity; // Wektor prędkości w osi X i Y

public:
    // Konstruktor tworzący pocisk w zadanym miejscu i nadający mu wektor lotu na podstawie kąta
    Bullet(float x, float y, float angle);

    // Nadpisane metody z klasy GameObject
    void update(float deltaTime) override;  // Aktualizuje pozycję pocisku bazując na prędkości
    void draw(sf::RenderWindow& window) override;   // Rysuje pocisk na ekranie
    sf::FloatRect getBounds() const override;   // Zwraca hitbox do sprawdzania, czy trafiono przeciwnika
};