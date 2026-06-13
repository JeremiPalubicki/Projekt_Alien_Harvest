#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

// Dołączenie wszystkich klas obiektów występujących w grze
#include "Runner.h"
#include "Boss.h"
#include "Player.h"
#include "Bullet.h"
#include "Alien.h"
#include "AlienBullet.h" 
#include "Medkit.h"      
#include "Obstacle.h" // NOWOŚĆ MILESTONE 8: Klasa przeszkód

// Definicja globalnych stanów gry - pozwala na łatwe przełączanie między menu, grą a ekranem porażki
enum GameState { MENU, PLAYING, GAMEOVER };

int main() {
    // Utworzenie głównego okna gry o wymiarach 800x600 pikseli
    // Zablokowane powiększanie okna
    sf::RenderWindow window(sf::VideoMode(800, 600), "Alien Harvest", sf::Style::Titlebar | sf::Style::Close);
    // Zegar do mierzenia czasu między klatkami (tzw. deltaTime)
    sf::Clock clock;

    // Inicjalizacja generatora liczb losowych na podstawie aktualnego czasu
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Gra uruchamia się w stanie MENU
    GameState currentState = MENU;

    // ==========================================
    // --- ŁADOWANIE ZASOBÓW (Grafiki i Czcionki) ---
    // ==========================================
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("assets/map1.png")) {
        std::cerr << "Blad ladowania mapy!" << std::endl;
    }
    sf::Sprite background(mapTexture);
    background.setScale(3.0f, 3.0f); // Skalujemy lekko mape, poniewaz teraz mamy kamere, wiec obszar jest wiekszy
    background.setPosition(-800, -600); // Przesuwamy srodek mapy

    sf::Texture alienTexture;
    if (!alienTexture.loadFromFile("assets/alien.png")) {
        std::cerr << "Blad ladowania kosmity!" << std::endl;
    }
    sf::Texture medkitTexture;
    if (!medkitTexture.loadFromFile("assets/medkit.png")) {
        std::cerr << "Blad ladowania apteczki!" << std::endl;
    }

    // --- NOWE ZMIENNE DO MILESTONE 8 ---
    sf::Texture treeTex;
    if (!treeTex.loadFromFile("assets/tree.png")) {
        std::cerr << "Blad ladowania drzewa! Utworz plik assets/tree.png" << std::endl;
    }

    std::vector<Obstacle> obstacles;
    // Generujemy kilka drzew na mapie (rozrzucone szeroko)
    obstacles.push_back(Obstacle(200, 300, treeTex));
    obstacles.push_back(Obstacle(-400, 800, treeTex));
    obstacles.push_back(Obstacle(1200, -200, treeTex));
    obstacles.push_back(Obstacle(800, 1000, treeTex));

    // KAMERA (Wielkosc naszego okna)
    sf::View camera(sf::FloatRect(0, 0, 800, 600));

    // ZASADY GRY I STOPER
    sf::Clock gameTimer;
    bool isGameWon = false;
    bool isBossSpawned = false;
    float finalTime = 0.0f;

    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Blad: Nie znaleziono pliku assets/font.ttf!" << std::endl;
    }

    // ==========================================
    // --- KONFIGURACJA TEKSTÓW (Interfejs / HUD) ---
    // ==========================================

    // Tekst wyświetlający statystyki gracza (lewy górny róg)
    sf::Text hudText;
    hudText.setFont(font);
    hudText.setCharacterSize(16);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10.0f, 10.0f);

    // Tekst używany na ekranach MENU, GAMEOVER i WYGRANEJ
    sf::Text centerText;
    centerText.setFont(font);
    centerText.setCharacterSize(20);
    centerText.setFillColor(sf::Color::Yellow);

    // ==========================================
    // --- ZMIENNE GLOBALNE GRY ---
    // ==========================================
    int killCount = 0;              // Liczba pokonanych wrogów
    int currentLevel = 1;           // Aktualny poziom trudności
    sf::Clock alienSpawnClock;      // Zegar odmierzający czas do pojawienia się nowego wroga
    float spawnCooldown = 1.5f;     // Czas (w sekundach) między spawnami wrogów
    float currentAlienSpeed = 100.0f; // Początkowa prędkość wrogów
    int currentAlienHp = 2;         // Początkowe punkty życia wrogów

    // Główny kontener przechowujący WSZYSTKIE obiekty w grze (Polimorfizm)
    std::vector<std::unique_ptr<GameObject>> entities;
    Player* player = nullptr; // Wskaźnik na gracza dla szybkiego dostępu

    // Funkcja (Lambda) resetująca wszystkie zmienne i czyszcząca mapę do stanu początkowego
    auto resetGame = [&]() {
        entities.clear(); // Usunięcie starych obiektów

        // Tworzymy nowego gracza na środku ekranu i dodajemy go do wektora
        auto playerPtr = std::make_unique<Player>(400.0f, 300.0f);
        player = playerPtr.get();
        entities.push_back(std::move(playerPtr));

        // Reset statystyk
        killCount = 0;
        currentLevel = 1;
        spawnCooldown = 1.5f;
        currentAlienSpeed = 100.0f;
        currentAlienHp = 2;
        mapTexture.loadFromFile("assets/map1.png");

        // Reset Milestone 8
        isGameWon = false;
        isBossSpawned = false;
        finalTime = 0.0f;
        gameTimer.restart();
        camera.setCenter(400.0f, 300.0f);
        };

    resetGame(); // Inicjalizacja gry przy pierwszym uruchomieniu programu

    // ==========================================
    // --- GŁÓWNA PĘTLA GRY ---
    // ==========================================
    while (window.isOpen()) {
        // Zmierz czas trwania ostatniej klatki w sekundach (niezbędne do płynnego ruchu niezależnie od FPS)
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        // --- OBSŁUGA ZDARZEŃ (Klawiatura / Mysz / Okno) ---
        while (window.pollEvent(event)) {
            // Zamknięcie okna krzyżykiem lub klawiszem ESC
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

            if (currentState == MENU) {
                // Przejście z Menu do Gry po wciśnięciu Enter
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    resetGame(); // Wymusza zresetowanie stopera
                    currentState = PLAYING;
                }
            }
            else if (currentState == PLAYING) {
                if (isGameWon) {
                    // Reset gry po WYGRANEJ za pomocą klawisza Enter
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        resetGame();
                        currentState = PLAYING;
                    }
                }
                else {
                    // Strzał gracza lewym przyciskiem myszy
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        if (!player->isDestroyed()) {
                            // Tworzy nowy pocisk i ustawia go w miejscu i rotacji gracza
                            entities.push_back(std::make_unique<Bullet>(
                                player->getPosition().x, player->getPosition().y, player->getRotation()
                            ));
                        }
                    }
                }
            }
            else if (currentState == GAMEOVER) {
                // Restart gry po przegranej za pomocą klawisza Enter
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    resetGame();
                    currentState = PLAYING;
                }
            }
        }

        // ==========================================
        // --- LOGIKA GRY (Wykonywana tylko w stanie PLAYING) ---
        // ==========================================
        if (currentState == PLAYING && !isGameWon) {

            // Obracanie gracza w stronę kursora myszy
            if (!player->isDestroyed()) {
                player->rotateTowardsMouse(window);
            }

            // MILESTONE 8: Zapisujemy pozycję gracza PRZED aktualizacją ruchu (potrzebne do blokowania o drzewa)
            sf::Vector2f oldPlayerPos = player->getPosition();

            // BUFOR BEZPIECZEŃSTWA: Przechowuje obiekty stworzone W TRAKCIE pętli
            std::vector<std::unique_ptr<GameObject>> newEntities;

            // Aktualizacja wszystkich obiektów na mapie
            for (size_t i = 0; i < entities.size(); ++i) {
                entities[i]->update(deltaTime);

                if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                    if (alien->canShoot() && currentLevel >= 1) {
                        sf::Vector2f pPos = player->getPosition();

                        newEntities.push_back(std::make_unique<AlienBullet>(
                            alien->getPosition().x, alien->getPosition().y, sf::Vector2f(pPos.x, pPos.y - 60.0f)
                        ));
                        newEntities.push_back(std::make_unique<AlienBullet>(
                            alien->getPosition().x, alien->getPosition().y, sf::Vector2f(pPos.x, pPos.y + 60.0f)
                        ));

                        alien->resetShootTimer();
                    }
                }
            }

            // MILESTONE 8: FIZYKA KOLIZJI Z DRZEWAMI
            if (!player->isDestroyed()) {
                for (const auto& obs : obstacles) {
                    // Sprawdzamy czy zaktualizowany hitbox nachodzi na drzewo
                    if (player->getBounds().intersects(obs.getGlobalBounds())) {
                        // Jesli wpadl w drzewo, cofnij go do starej pozycji!
                        player->setPosition(oldPlayerPos.x, oldPlayerPos.y);
                        break;
                    }
                }
            }

            // --- NOWY SPAWNER WROGÓW I BOSSA (Milestone 8) ---
            if (!isBossSpawned) {
                if (alienSpawnClock.getElapsedTime().asSeconds() > spawnCooldown) {
                    if (!player->isDestroyed()) {

                        // Co 15 zabojstw podnosimy trudnosc (zaktualizowany level system)
                        currentLevel = 1 + (killCount / 15);

                        if (killCount >= 50) {
                            // WYCZYŚĆ MAPĘ ZE ZWYKŁYCH KOSMITÓW I SPAWNUJ BOSSA
                            // Zostawiamy gracza, medkity i pociski, ale niszczymy kosmitów
                            entities.erase(std::remove_if(entities.begin(), entities.end(),
                                [](const std::unique_ptr<GameObject>& obj) {
                                    return dynamic_cast<Alien*>(obj.get()) != nullptr;
                                }), entities.end());

                            // Tworzymy Bossa zaraz za ekranem kamery (np. po prawej stronie od gracza)
                            entities.push_back(std::make_unique<Boss>(
                                player->getPosition().x + 500.0f, player->getPosition().y, player, alienTexture, &entities
                            ));
                            isBossSpawned = true;
                        }
                        else {
                            // STARY SYSTEM SPAWNOWANIA (Fale przed osiągnięciem 50 zabić)
                            int aliensToSpawn = 1 + (currentLevel / 3);

                            for (int k = 0; k < aliensToSpawn; ++k) {
                                float spawnX = 0.0f;
                                float spawnY = 0.0f;

                                // Respią się wokół gracza (żeby uwzględnić przesuniętą kamerę!)
                                sf::Vector2f center = player->getPosition();
                                int edge = std::rand() % 4;

                                if (edge == 0) { // GÓRA 
                                    spawnX = center.x - 400.0f + (std::rand() % 800);
                                    spawnY = center.y - 350.0f;
                                }
                                else if (edge == 1) { // PRAWO
                                    spawnX = center.x + 450.0f;
                                    spawnY = center.y - 300.0f + (std::rand() % 600);
                                }
                                else if (edge == 2) { // DÓŁ 
                                    spawnX = center.x - 400.0f + (std::rand() % 800);
                                    spawnY = center.y + 350.0f;
                                }
                                else { // LEWO
                                    spawnX = center.x - 450.0f;
                                    spawnY = center.y - 300.0f + (std::rand() % 600);
                                }

                                int chance = std::rand() % 100;

                                if (currentLevel >= 2 && chance < 35) {
                                    entities.push_back(std::make_unique<Runner>(
                                        spawnX, spawnY, player, alienTexture
                                    ));
                                }
                                else {
                                    entities.push_back(std::make_unique<Alien>(
                                        spawnX, spawnY, player, alienTexture, currentAlienSpeed, currentAlienHp
                                    ));
                                }
                            }
                        }
                    }
                    alienSpawnClock.restart();
                }
            }

            // --- SYSTEM KOLIZJI I ZDERZEŃ ---
            for (size_t i = 0; i < entities.size(); ++i) {

                // 1. Zderzenie: Kula Gracza -> Kosmita
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

                                    if (std::rand() % 100 < 15) {
                                        newEntities.push_back(std::make_unique<Medkit>(
                                            alien->getPosition().x, alien->getPosition().y, medkitTexture
                                        ));
                                    }

                                    // LEVELOWANIE JEST TERAZ W ZALEZNOSCI OD SPEDRUNA W SPAWNERZE, wiec zwiekszamy tylko statystyki
                                    if (killCount % 15 == 0 && killCount <= 50) {
                                        std::string mapPath = "assets/map" + std::to_string(currentLevel > 5 ? 5 : currentLevel) + ".png";
                                        mapTexture.loadFromFile(mapPath);

                                        spawnCooldown -= 0.35f;
                                        currentAlienSpeed += 30.0f;
                                        currentAlienHp += 1;

                                        if (spawnCooldown < 0.1f) spawnCooldown = 0.1f;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

                // 2. Zderzenie: Kosmita -> Gracz
                if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                    if (alien->isDestroyed() || player->isDestroyed()) continue;
                    if (alien->getBounds().intersects(player->getBounds())) {
                        alien->destroy();
                        player->takeDamage(20);
                    }
                }

                // 3. Zderzenie: Kula Wroga -> Gracz
                if (auto* alienBullet = dynamic_cast<AlienBullet*>(entities[i].get())) {
                    if (alienBullet->isDestroyed() || player->isDestroyed()) continue;
                    if (alienBullet->getBounds().intersects(player->getBounds())) {
                        alienBullet->destroy();
                        player->takeDamage(10);
                    }
                }

                // 4. Zderzenie: Apteczka -> Gracz
                if (auto* medkit = dynamic_cast<Medkit*>(entities[i].get())) {
                    if (medkit->isDestroyed() || player->isDestroyed()) continue;
                    if (medkit->getBounds().intersects(player->getBounds())) {
                        medkit->destroy();
                        player->heal(30);
                    }
                }
            }

            for (auto& newEnt : newEntities) {
                entities.push_back(std::move(newEnt));
            }

            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isDestroyed(); }), entities.end());

            // MILESTONE 8: Sprawdzenie, czy Boss został pokonany (warunek wygranej)
            if (isBossSpawned && !isGameWon) {
                bool bossAlive = false;
                for (const auto& ent : entities) {
                    if (dynamic_cast<Boss*>(ent.get()) != nullptr && !ent->isDestroyed()) {
                        bossAlive = true;
                        break;
                    }
                }
                if (!bossAlive) { // Jeśli bossa już nie ma w wektorze żywych:
                    isGameWon = true;
                    finalTime = gameTimer.getElapsedTime().asSeconds();
                }
            }

            // Aktualizacja interfejsu (Timer i Licznik dodane!)
            hudText.setString("HP: " + std::to_string(player->getHealth()) +
                " | Czas: " + std::to_string((int)gameTimer.getElapsedTime().asSeconds()) + "s" +
                " | Zabici: " + std::to_string(killCount) + "/50");

            if (player->getHealth() <= 0) {
                currentState = GAMEOVER;
            }
        }

        // ==========================================
        // --- RENDEROWANIE (Podwójne okno) ---
        // ==========================================
        window.clear(sf::Color::Black);

        if (currentState == PLAYING) {

            // --- 1. RYSOWANIE ŚWIATA (Przesuwanie z graczem) ---
            if (!player->isDestroyed()) {
                camera.setCenter(player->getPosition()); // Kamera zawsze na farmerze
            }
            window.setView(camera);

            window.draw(background);

            for (const auto& obs : obstacles) {
                obs.draw(window); // Rysowanie przeszkód
            }

            for (auto& entity : entities) {
                entity->draw(window);
            }

            // --- 2. RYSOWANIE INTERFEJSU (UI) ---
            // Resetujemy kamerę na standardową, żeby tekst był "przyklejony" do ekranu
            window.setView(window.getDefaultView());

            if (isGameWon) {
                centerText.setString("WYGRANA!\nTwoj czas to: " + std::to_string(finalTime) + "s\n\nWcisnij ENTER aby zagrac ponownie");
                sf::FloatRect textRect = centerText.getLocalBounds();
                centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f);
                window.draw(centerText);
            }
            else {
                window.draw(hudText); // Jeśli gra trwa, po prostu narysuj HUD
            }

        }
        else if (currentState == MENU) {
            window.setView(window.getDefaultView()); // Menu ma bazową kamerę
            centerText.setString("ALIEN HARVEST: SPEEDRUN MODE\n\nWcisnij ENTER aby rozpoczac");
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f);
            window.draw(centerText);
        }
        else if (currentState == GAMEOVER) {
            window.setView(window.getDefaultView()); // Gameover ma bazową kamerę
            centerText.setString("GAME OVER!\nZlikwidowani kosmici: " + std::to_string(killCount) + "\n\nWcisnij ENTER aby zagrac ponownie");
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f);
            window.draw(centerText);
        }

        window.display(); // Wyświetlenie gotowej klatki na monitorze
    }
    return 0; // Prawidłowe zakończenie programu
}