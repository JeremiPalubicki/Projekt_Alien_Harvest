#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "Player.h"
#include "Bullet.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Farm Defense");
    sf::Clock clock;

    // GŁÓWNY KONTENER NA WSZYSTKO 
    std::vector<std::unique_ptr<GameObject>> entities;

    auto playerPtr = std::make_unique<Player>(400.0f, 300.0f);
    Player* player = playerPtr.get();
    entities.push_back(std::move(playerPtr));

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