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

// Definicja stanów gry
enum GameState { MENU, PLAYING, GAMEOVER };

int main() {
    // Utworzenie okna gry
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Harvest");
    sf::Clock clock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    GameState currentState = MENU;

    // --- ŁADOWANIE ZASOBÓW ---
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("assets/map1.png")) {
        std::cerr << "Blad ladowania mapy!" << std::endl;
    }
    sf::Sprite background(mapTexture);

    sf::Texture alienTexture;
    if (!alienTexture.loadFromFile("assets/alien.png")) {
        std::cerr << "Blad ladowania kosmity!" << std::endl;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Blad: Nie znaleziono pliku assets/font.ttf!" << std::endl;
    }

    // --- KONFIGURACJA TEKSTÓW (UI) ---
    sf::Text hudText;
    hudText.setFont(font);
    // Zmniejszono rozmiar tekstu HUD z uwagi na szeroką czcionkę
    hudText.setCharacterSize(16);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10.0f, 10.0f); // Lewy górny róg

    sf::Text centerText;
    centerText.setFont(font);
    centerText.setCharacterSize(20); // Mniejszy rozmiar, żeby mieć pewność, że się zmieści
    centerText.setFillColor(sf::Color::Yellow);

    // --- ZMIENNE GRY ---
    int killCount = 0;
    int currentLevel = 1;
    sf::Clock alienSpawnClock;
    float spawnCooldown = 1.5f;
    float currentAlienSpeed = 100.0f;
    int currentAlienHp = 2;

    std::vector<std::unique_ptr<GameObject>> entities;
    Player* player = nullptr;

    // Funkcja resetująca grę (Lambda)
    auto resetGame = [&]() {
        entities.clear();
        auto playerPtr = std::make_unique<Player>(400.0f, 300.0f);
        player = playerPtr.get();
        entities.push_back(std::move(playerPtr));

        killCount = 0;
        currentLevel = 1;
        spawnCooldown = 1.5f;
        currentAlienSpeed = 100.0f;
        currentAlienHp = 2;
        mapTexture.loadFromFile("assets/map1.png");
        };

    resetGame(); // Inicjalizacja przy starcie

    // --- GŁÓWNA PĘTLA GRY ---
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

            // Interakcje zależne od stanu gry
            if (currentState == MENU) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentState = PLAYING;
                }
            }
            else if (currentState == PLAYING) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (!player->isDestroyed()) {
                        entities.push_back(std::make_unique<Bullet>(
                            player->getPosition().x, player->getPosition().y, player->getRotation()
                        ));
                    }
                }
            }
            else if (currentState == GAMEOVER) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    resetGame();
                    currentState = PLAYING;
                }
            }
        }

        // --- LOGIKA GRY (Tylko gdy gramy) ---
        if (currentState == PLAYING) {
            if (!player->isDestroyed()) {
                player->rotateTowardsMouse(window);
            }

            for (auto& entity : entities) {
                entity->update(deltaTime);
            }

            // Spawner wrogów
            if (alienSpawnClock.getElapsedTime().asSeconds() > spawnCooldown) {
                float randomY = static_cast<float>(std::rand() % 500 + 50);
                if (!player->isDestroyed()) {
                    entities.push_back(std::make_unique<Alien>(
                        800.0f, randomY, player, alienTexture, currentAlienSpeed, currentAlienHp
                    ));
                }
                alienSpawnClock.restart();
            }

            // Kolizje i poziomy
            for (size_t i = 0; i < entities.size(); ++i) {
                if (auto* bullet = dynamic_cast<Bullet*>(entities[i].get())) {
                    if (bullet->isDestroyed()) continue;
                    for (size_t j = 0; j < entities.size(); ++j) {
                        if (auto* alien = dynamic_cast<Alien*>(entities[j].get())) {
                            if (alien->isDestroyed()) continue;
                            if (bullet->getBounds().intersects(alien->getBounds())) {
                                bullet->destroy();
                                alien->takeDamage(1);

                                if (alien->isDestroyed()) {
                                    killCount++;
                                    int newLevel = 1 + (killCount / 20);
                                    if (newLevel > currentLevel && newLevel <= 5) {
                                        currentLevel = newLevel;
                                        std::string mapPath = "assets/map" + std::to_string(currentLevel) + ".png";
                                        mapTexture.loadFromFile(mapPath);
                                        spawnCooldown -= 0.2f;
                                        currentAlienSpeed += 25.0f;
                                        if (currentLevel >= 3) currentAlienHp = 3;
                                        if (spawnCooldown < 0.3f) spawnCooldown = 0.3f;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

                if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                    if (alien->isDestroyed() || player->isDestroyed()) continue;
                    if (alien->getBounds().intersects(player->getBounds())) {
                        alien->destroy();
                        player->takeDamage(20);
                    }
                }
            }

            // Usuwanie zniszczonych obiektów
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isDestroyed(); }), entities.end());

            // Aktualizacja tekstu HUD
            hudText.setString("HP: " + std::to_string(player->getHealth()) +
                " | Poziom: " + std::to_string(currentLevel) +
                " | Zlikwidowani: " + std::to_string(killCount));

            // Sprawdzanie śmierci
            if (player->getHealth() <= 0) {
                currentState = GAMEOVER;
            }
        }

        // --- RENDEROWANIE ---
        window.clear(sf::Color::Black);

        if (currentState == PLAYING) {
            window.draw(background);
            for (auto& entity : entities) {
                entity->draw(window);
            }
            window.draw(hudText); // Rysowanie UI w lewym górnym rogu
        }
        else if (currentState == MENU) {
            centerText.setString("ALIEN HARVEST\n\nWcisnij ENTER aby rozpoczac");

            // Dynamiczne wyśrodkowanie tekstu
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f); // Dokładny środek okna 800x600

            window.draw(centerText);
        }
        else if (currentState == GAMEOVER) {
            centerText.setString("GAME OVER!\nZlikwidowani kosmici: " + std::to_string(killCount) + "\n\nWcisnij ENTER aby zagrac ponownie");

            // Dynamiczne wyśrodkowanie tekstu
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f); // Dokładny środek okna 800x600

            window.draw(centerText);
        }

        window.display();
    }
    return 0;
}