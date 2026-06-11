#pragma once
#include "Alien.h"
#include <vector>
#include <memory>

class Boss : public Alien {
private:
    std::vector<std::unique_ptr<GameObject>>* gameEntities;
    sf::Texture* alienTex;
    float spawnTimer;

public:
    Boss(float x, float y, Player* playerTarget, sf::Texture& tex, std::vector<std::unique_ptr<GameObject>>* entities);
    void update(float deltaTime) override;
};