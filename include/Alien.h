#pragma once
#include "GameObject.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

/// Podstawowa klasa przeciwnika, który podąża za graczem.
class Alien : public GameObject {
protected:
    sf::Clock shootTimer;
    float shootCooldown;
    sf::Sprite sprite;  // Wygląd graficzny przeciwnika
    float speed;    // Prędkość poruszania się w stronę gracza
    Player* target; // Wskaźnik na obiekt gracza, za którym podąża ten kosmita
    int hp; // Punkty życia przeciwnika

public:
    // Konstruktor przyjmuje cel (gracza), teksturę oraz opcjonalnie własną prędkość i HP
    Alien(float x, float y, Player* playerTarget, sf::Texture& tex, float moveSpeed = 100.0f, int maxHp = 2);
    bool canShoot() const;
    void resetShootTimer();
    // Nadpisane metody z klasy GameObject
    void update(float deltaTime) override;  // Odpowiada za ruch w kierunku gracza
    void draw(sf::RenderWindow& window) override;   // Rysuje kosmitę
    sf::FloatRect getBounds() const override;   // Hitbox do odbierania obrażeń od pocisków

    /// Zmniejsza HP przeciwnika o podaną wartość i oznacza go jako zniszczonego, gdy HP spadnie <= 0
    void takeDamage(int amount);
};