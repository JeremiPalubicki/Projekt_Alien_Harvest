#include <SFML/Graphics.hpp>
#include "Player.h"

int main() {
    // odpalamy okno
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Harvest - setup");
    
    // robimy zegar do liczenia deltaTime
    sf::Clock clock;
    
    // testowy gracz na srodku ekranu
    Player player(400.0f, 300.0f);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
        }

        // update logiki
        player.rotateTowardsMouse(window);
        player.update(deltaTime);

        // rysowanie
        window.clear(sf::Color::Black);
        player.draw(window);
        window.display();
    }
    return 0;
}