#pragma once
#include "Alien.h"
#include <vector>
#include <memory>

/// Klasa reprezentująca silniejszego przeciwnika (Bossa), który posiada dodatkowe umiejętności.
class Boss : public Alien {
private:
    // Wskaźnik na globalną listę wszystkich obiektów w grze - dzięki temu Boss może np. "tworzyć" innych kosmitów (miniony)
    std::vector<std::unique_ptr<GameObject>>* gameEntities;
    sf::Texture* alienTex;  // Wskaźnik na teksturę zwykłego kosmity (do spawnowania pomocników)
    float spawnTimer;   // Licznik czasu odliczający do stworzenia kolejnego miniona

public:
    // Konstruktor Bossa - wymaga referencji do wektora entities, by móc dodawać do niego nowe obiekty
    Boss(float x, float y, Player* playerTarget, sf::Texture& tex, std::vector<std::unique_ptr<GameObject>>* entities);

    // Nadpisana metoda update, która oprócz ruchu realizuje również logikę ataków/spawnowania wrogów
    void update(float deltaTime) override;
};