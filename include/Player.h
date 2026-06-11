#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

/// Klasa reprezentująca postać gracza sterowaną przez użytkownika./// Klasa reprezentująca postać gracza sterowaną przez użytkownika.
class Player : public GameObject {
private:
    sf::Texture texture;    // Tekstura przypisana do gracza
    sf::Sprite sprite;  // Wygląd graficzny gracza
    float speed;    // Prędkość poruszania się gracza
    int health; // Aktualne punkty życia
    int maxHealth;  // Maksymalna ilość punktów życia
    float aimAngle; // Kąt, pod którym gracz jest obrócony (np. w stronę kursora)
public:
    // Konstruktor inicjalizujący gracza na podanych współrzędnych
    Player(float x, float y);

    // Nadpisane metody z klasy GameObject
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    /// Obraca sprite'a gracza w kierunku aktualnej pozycji kursora myszy
    void rotateTowardsMouse(const sf::RenderWindow& window);

    /// Odbiera graczowi określoną ilość punktów życia
    void takeDamage(int amount);

    // Gettery do pobierania stanu zdrowia i rotacji
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }

 
    float getRotation() const { return aimAngle; }
};