#pragma once
#include "Alien.h"

class Runner : public Alien {
private:
    float timeAlive;

public:
    Runner(float x, float y, Player* playerTarget, sf::Texture& tex);
    void update(float deltaTime) override;
};