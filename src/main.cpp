#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>



#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "Bullet.h"
#include "Alien.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Farm Defense");
    sf::Clock clock;

    // Inicjalizacja narzedzi losowania i zegara spawnera przed pętla
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::Clock alienSpawnClock;
    float spawnCooldown = 1.5f;

    // GŁÓWNY KONTENER NA WSZYSTKO 
    std::vector<std::unique_ptr<GameObject>> entities;

    auto playerPtr = std::make_unique<Player>(400.0f, 300.0f);
    Player* player = playerPtr.get();
    entities.push_back(std::move(playerPtr));
    
    // testowy obcy, dajemy mu wskaznik na gracza zeby mial za kim latac
    entities.push_back(std::make_unique<Alien>(50.0f, 50.0f, player));
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

            // Strzelanie (Lewy Przycisk Myszy)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (!player->isDestroyed()) {
                    entities.push_back(std::make_unique<Bullet>(
                        player->getPosition().x,
                        player->getPosition().y,
                        player->getRotation()
                    ));
                }
            }
        }

        if (!player->isDestroyed()) {
            player->rotateTowardsMouse(window);
        }

        for (auto& entity : entities) {
            entity->update(deltaTime);
        }

        // ---  MIEJSCE NA SPAWNER I KOLIZJE ---
        
        // 1. SPAWNER OBCYCH
        if (alienSpawnClock.getElapsedTime().asSeconds() > spawnCooldown) {
            float randomY = static_cast<float>(std::rand() % 500 + 50); // Losowanie z prawej krawędzi
            if (!player->isDestroyed()) {
                entities.push_back(std::make_unique<Alien>(800.0f, randomY, player));
            }
            alienSpawnClock.restart();
        }

        // 2. SYSTEM KOLIZJI I OBRAŻEŃ (Wymóg: dynamic_cast)
        for (size_t i = 0; i < entities.size(); ++i) {
            
            // Kolizja: POCISK -> KOSMITA
            if (auto* bullet = dynamic_cast<Bullet*>(entities[i].get())) {
                if (bullet->isDestroyed()) continue;

                for (size_t j = 0; j < entities.size(); ++j) {
                    if (auto* alien = dynamic_cast<Alien*>(entities[j].get())) {
                        if (alien->isDestroyed()) continue;

                        if (bullet->getBounds().intersects(alien->getBounds())) {
                            bullet->destroy(); // Pocisk znika od razu
                            alien->takeDamage(1); // Obcy traci 1 HP (padnie na 2 strzały)
                            break; 
                        }
                    }
                }
            }

            // Kolizja: KOSMITA -> GRACZ
            if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                if (alien->isDestroyed() || player->isDestroyed()) continue;
                
                // Jeśli obcy dotknie farmera:
                if (alien->getBounds().intersects(player->getBounds())) {
                    alien->destroy(); // Kosmita wybucha przy kontakcie
                    player->takeDamage(20); // Zabiera 20 HP z 100
                    // INFO(Oliwier): Mateusz, zrób z tym później ładne UI w lewym rogu
                }
            }
        }
        

        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isDestroyed(); }),
            entities.end()
        );

        window.clear(sf::Color::Black);
        for (auto& entity : entities) {
            entity->draw(window);
        }
        window.display();
    }
    return 0;
}