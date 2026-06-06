#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

#include "Player.h"
#include "Bullet.h"
#include "Alien.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Harvest - Milestone 4B");
    sf::Clock clock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- SYSTEM ZARZĄDZANIA FASEM / POZIOMEM ---
    int killCount = 0;
    int currentLevel = 1;

    // Statystyki przeciwnikow, ktore beda rosly z kazdym poziomem
    sf::Clock alienSpawnClock;
    float spawnCooldown = 1.5f;
    float currentAlienSpeed = 100.0f;
    int currentAlienHp = 2;

    // --- ŁADOWANIE ZASOBÓW (ASSETS) ---
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("assets/map1.png")) {
        std::cerr << "Blad ladowania assets/map1.png" << std::endl;
    }
    sf::Sprite background(mapTexture);

    sf::Texture alienTexture;
    if (!alienTexture.loadFromFile("assets/alien.png")) {
        std::cerr << "Blad ladowania assets/alien.png" << std::endl;
    }

    // Główny wektor gry na inteligentnych wskaźnikach
    std::vector<std::unique_ptr<GameObject>> entities;

    // Tworzenie gracza
    auto playerPtr = std::make_unique<Player>(400.0f, 300.0f);
    Player* player = playerPtr.get();
    entities.push_back(std::move(playerPtr));

    std::cout << "=== GRA ROZPOCZETA ===" << std::endl;
    std::cout << "Poziom 1: Zabij 20 kosmitow aby awansowac." << std::endl;

    // --- GŁÓWNA PĘTLA GRY ---
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        // Obsługa zdarzeń
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

            // Strzelanie
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

        // Aktualizacja logiki wszystkich obiektów
        for (auto& entity : entities) {
            entity->update(deltaTime);
        }

        // --- ZAAWANSOWANY SPAWNER WROGÓW ---
        if (alienSpawnClock.getElapsedTime().asSeconds() > spawnCooldown) {
            float randomY = static_cast<float>(std::rand() % 500 + 50);
            if (!player->isDestroyed()) {
                // Przekazujemy teksturę oraz aktualne statystyki dla danego poziomu
                entities.push_back(std::make_unique<Alien>(
                    800.0f, randomY, player, alienTexture, currentAlienSpeed, currentAlienHp
                ));
            }
            alienSpawnClock.restart();
        }

        // --- SYSTEM KOLIZJI I PROGRESJI ---
        for (size_t i = 0; i < entities.size(); ++i) {

            // Kolizja: Pocisk uderza Kosmitę
            if (auto* bullet = dynamic_cast<Bullet*>(entities[i].get())) {
                if (bullet->isDestroyed()) continue;

                for (size_t j = 0; j < entities.size(); ++j) {
                    if (auto* alien = dynamic_cast<Alien*>(entities[j].get())) {
                        if (alien->isDestroyed()) continue;

                        if (bullet->getBounds().intersects(alien->getBounds())) {
                            bullet->destroy();
                            alien->takeDamage(1);

                            // Logika zabójstwa i wbijania poziomu
                            if (alien->isDestroyed()) {
                                killCount++;

                                int newLevel = 1 + (killCount / 20);
                                if (newLevel > currentLevel && newLevel <= 5) {
                                    currentLevel = newLevel;

                                    // Zmiana mapy
                                    std::string mapPath = "assets/map" + std::to_string(currentLevel) + ".png";
                                    mapTexture.loadFromFile(mapPath);

                                    // Skalowanie trudności
                                    spawnCooldown -= 0.2f;
                                    currentAlienSpeed += 25.0f;
                                    if (currentLevel >= 3) currentAlienHp = 3; // Od 3 poziomu kosmici mają 3 HP

                                    if (spawnCooldown < 0.3f) spawnCooldown = 0.3f; // Limit prędkości respawnu

                                    std::cout << "\n=== AWANS NA POZIOM " << currentLevel << " ===" << std::endl;
                                    std::cout << "Kosmici sa teraz szybsi i grozniejsi!" << std::endl;
                                }
                            }
                            break;
                        }
                    }
                }
            }

            // Kolizja: Kosmita uderza Gracza
            if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                if (alien->isDestroyed() || player->isDestroyed()) continue;

                if (alien->getBounds().intersects(player->getBounds())) {
                    alien->destroy();
                    player->takeDamage(20);
                }
            }
        }

        // Czyszczenie pamięci z usuniętych obiektów (STL algorithms)
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isDestroyed(); }),
            entities.end()
        );

        // --- RENDEROWANIE EKRANU ---
        window.clear(sf::Color::Black);

        window.draw(background); // Mapa zawsze na samym dnie

        for (auto& entity : entities) {
            entity->draw(window);
        }

        window.display();
    }
    return 0;
}