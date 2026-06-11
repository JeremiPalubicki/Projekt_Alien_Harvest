#pragma once
#include "Alien.h"

/// Specjalny typ przeciwnika, np. poruszający się szybciej lub po specjalnej trajektorii.
class Runner : public Alien {
private:
    float timeAlive;

public:
    // Konstruktor przekazujący parametry do klasy bazowej Alien
    Runner(float x, float y, Player* playerTarget, sf::Texture& tex);
    
    // Nadpisana metoda update, implementująca własny, unikalny sposób poruszania się
    void update(float deltaTime) override;
};