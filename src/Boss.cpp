#include "Boss.h"

// Konstruktor bazowy: predkosc 50 (wolny), hp 50 (wielki)
Boss::Boss(float x, float y, Player* playerTarget, sf::Texture& tex, std::vector<std::unique_ptr<GameObject>>* entities)
    : Alien(x, y, playerTarget, tex, 50.0f, 50), gameEntities(entities), alienTex(&tex), spawnTimer(0.0f) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    float targetSize = 110.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    // Fioletowy odcień
    sprite.setColor(sf::Color(200, 50, 255));
}

void Boss::update(float deltaTime) {
    if (!target || target->isDestroyed() || isDestroyed()) return;

    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Zatrzymuje się, gdy jest w odległości 350 pikseli od gracza
    if (distance > 350.0f) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;
    }

    sprite.setPosition(position);

    // Wypluwanie kosmitów co 2.5 sekundy
    spawnTimer += deltaTime;
    if (spawnTimer >= 2.5f) {
        spawnTimer = 0.0f;
        
        gameEntities->push_back(std::make_unique<Alien>(
            position.x, position.y, target, *alienTex, 150.0f, 2
        ));
    }
}