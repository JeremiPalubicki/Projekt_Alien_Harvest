#include "Boss.h"

// Konstruktor bazowy: inicjalizuje bossa z mniejszą prędkością (50.0f) i dużą ilością HP (50)
Boss::Boss(float x, float y, Player* playerTarget, sf::Texture& tex, std::vector<std::unique_ptr<GameObject>>* entities)
    : Alien(x, y, playerTarget, tex, 50.0f, 50), gameEntities(entities), alienTex(&tex), spawnTimer(0.0f) {

    // Skalowanie bossa do znacznie większego rozmiaru (110 pikseli)
    sf::FloatRect bounds = sprite.getLocalBounds();
    float targetSize = 110.0f;
    sprite.setScale(targetSize / bounds.width, targetSize / bounds.height);

    // Nadanie fioletowego odcienia, aby wizualnie odróżnić go od zwykłych przeciwników
    sprite.setColor(sf::Color(200, 50, 255));
}

void Boss::update(float deltaTime) {
    // Weryfikacja stanu celu
    if (!target || target->isDestroyed() || isDestroyed()) return;

    // Obliczanie wektora kierunkowego i dystansu do gracza
    sf::Vector2f targetPos = target->getPosition();
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Boss porusza się tylko wtedy, gdy odległość od gracza jest większa niż 350 pikseli (stara się trzymać dystans)
    if (distance > 350.0f) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        position.x += dirX * speed * deltaTime;
        position.y += dirY * speed * deltaTime;
    }

    sprite.setPosition(position);

    // Logika spawnowania mniejszych kosmitów
    spawnTimer += deltaTime;
    if (spawnTimer >= 2.5f) {   // Co 2.5 sekundy
        spawnTimer = 0.0f;
        
        // Dodanie nowego, szybszego przeciwnika do głównego wektora gry
        gameEntities->push_back(std::make_unique<Alien>(
            position.x, position.y, target, *alienTex, 150.0f, 2
        ));
    }
}