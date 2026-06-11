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
#include "AlienBullet.h" // Klasa pocisku wroga (Milestone 7)
#include "Medkit.h"      // Klasa apteczki (Milestone 7)

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

    sf::Texture alienTexture;
    if (!alienTexture.loadFromFile("assets/alien.png")) {
        std::cerr << "Blad ladowania kosmity!" << std::endl;
    }
    sf::Texture medkitTexture;
    if (!medkitTexture.loadFromFile("assets/medkit.png")) {
        std::cerr << "Blad ladowania apteczki!" << std::endl;
    }

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

    // Tekst używany na ekranach MENU i GAMEOVER (wyśrodkowany)
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
                    currentState = PLAYING;
                }
            }
            else if (currentState == PLAYING) {
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
        if (currentState == PLAYING) {

            // Obracanie gracza w stronę kursora myszy
            if (!player->isDestroyed()) {
                player->rotateTowardsMouse(window);
            }

            // BUFOR BEZPIECZEŃSTWA: Przechowuje obiekty stworzone W TRAKCIE pętli (np. nowe strzały wroga).
            // Zapobiega to tzw. "iterator invalidation" (crashe programu przy dodawaniu elementów do aktualnie czytanego wektora)
            std::vector<std::unique_ptr<GameObject>> newEntities;

            // Aktualizacja wszystkich obiektów na mapie (ruch kosmitów, lot pocisków itp.)
            for (size_t i = 0; i < entities.size(); ++i) {
                entities[i]->update(deltaTime);

                // NOWOŚĆ: Strzelanie kosmitów
               // NOWOŚĆ: Strzelanie kosmitów (Podwójny strzał - nożyce)
                if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                    if (alien->canShoot() && currentLevel >= 1) {
                        // Pobieramy aktualną pozycję gracza
                        sf::Vector2f pPos = player->getPosition();

                        // Pocisk 1: Celuje lekko nad głowę gracza (Y - 60)
                        newEntities.push_back(std::make_unique<AlienBullet>(
                            alien->getPosition().x, alien->getPosition().y, sf::Vector2f(pPos.x, pPos.y - 60.0f)
                        ));

                        // Pocisk 2: Celuje lekko pod nogi gracza (Y + 60)
                        newEntities.push_back(std::make_unique<AlienBullet>(
                            alien->getPosition().x, alien->getPosition().y, sf::Vector2f(pPos.x, pPos.y + 60.0f)
                        ));

                        alien->resetShootTimer(); // Resetujemy stoper
                    }
                }
            }

            // --- SPAWNER WROGÓW ---
            if (alienSpawnClock.getElapsedTime().asSeconds() > spawnCooldown) {

                if (!player->isDestroyed()) {

                    // NOWOŚĆ 1: Więcej wrogów! Obliczamy, ilu kosmitów zrespić w tym samym momencie
                    // Poziom 1 i 2: 1 kosmita. Poziom 3 i 4: 2 kosmitów na raz. Poziom 5: 3 naraz!
                    int aliensToSpawn = 1 + (currentLevel / 3);

                    for (int k = 0; k < aliensToSpawn; ++k) {
                        float spawnX = 0.0f;
                        float spawnY = 0.0f;

                        // NOWOŚĆ 2: Losujemy krawędź ekranu (0 - Góra, 1 - Prawo, 2 - Dół, 3 - Lewo)
                        int edge = std::rand() % 4;

                        if (edge == 0) { // GÓRA (Wychodzą zza górnej krawędzi)
                            spawnX = static_cast<float>(std::rand() % 800);
                            spawnY = -50.0f;
                        }
                        else if (edge == 1) { // PRAWO (Wychodzą zza prawej krawędzi)
                            spawnX = 850.0f;
                            spawnY = static_cast<float>(std::rand() % 600);
                        }
                        else if (edge == 2) { // DÓŁ (Wychodzą zza dolnej krawędzi)
                            spawnX = static_cast<float>(std::rand() % 800);
                            spawnY = 650.0f;
                        }
                        else { // LEWO (Wychodzą zza lewej krawędzi)
                            spawnX = -50.0f;
                            spawnY = static_cast<float>(std::rand() % 600);
                        }

                        int chance = std::rand() % 100;

                        // Dodajemy wroga z wylosowanymi koordynatami (spawnX, spawnY) zamiast sztywnego 800.0f
                        if (currentLevel >= 5 && chance < 10) {
                            entities.push_back(std::make_unique<Boss>(
                                spawnX, spawnY, player, alienTexture, &entities
                            ));
                        }
                        else if (currentLevel >= 2 && chance < 35) {
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
                alienSpawnClock.restart(); // Reset zegara spawnera
            }

            // --- SYSTEM KOLIZJI I ZDERZEŃ ---
            for (size_t i = 0; i < entities.size(); ++i) {

                // 1. Zderzenie: Kula Gracza -> Kosmita
                if (auto* bullet = dynamic_cast<Bullet*>(entities[i].get())) {
                    if (bullet->isDestroyed()) continue; // Ignoruj zniszczone pociski
                    for (size_t j = 0; j < entities.size(); ++j) {
                        if (auto* alien = dynamic_cast<Alien*>(entities[j].get())) {
                            if (alien->isDestroyed()) continue;

                            // Sprawdzenie nachodzenia na siebie tekstur (zderzenie)
                            if (bullet->getBounds().intersects(alien->getBounds())) {
                                bullet->destroy(); // Zniszcz pocisk
                                alien->takeDamage(1); // Zabierz kosmicie 1 HP

                                // Jeśli kosmita zginie w wyniku strzału
                                if (alien->isDestroyed()) {
                                    killCount++; // Zwiększ wynik

                                    // SYSTEM LOOTU: Szansa na drop apteczki (15% szans) po śmierci kosmity
                                    // SYSTEM LOOTU: Szansa na drop apteczki (15% szans) po śmierci kosmity
                                    if (std::rand() % 100 < 15) {
                                        newEntities.push_back(std::make_unique<Medkit>(
                                            alien->getPosition().x, alien->getPosition().y, medkitTexture
                                        ));
                                    }

                                    // SYSTEM LEVELOWANIA: Co 20 zabójstw awansuj na nowy poziom
                                   // Awansujemy szybciej - co 15 zabójstw zamiast 20
                                    int newLevel = 1 + (killCount / 20);

                                    if (newLevel > currentLevel && newLevel <= 5) {
                                        currentLevel = newLevel;
                                        std::string mapPath = "assets/map" + std::to_string(currentLevel) + ".png";
                                        mapTexture.loadFromFile(mapPath);

                                        // BRUTALNY SYSTEM:
                                        spawnCooldown -= 0.35f; // Kosmici wychodzą szybciej
                                        currentAlienSpeed += 30.0f; // Biegają szybciej
                                        currentAlienHp += 1; // Z każdym levelem wrogowie stają się grubszymi tarczami (mają więcej HP!)

                                        if (spawnCooldown < 0.1f) spawnCooldown = 0.1f;
                                    }
                                }
                                break; // Kula zniknęła, przerwij sprawdzanie dla niej kolejnych kosmitów
                            }
                        }
                    }
                }

                // 2. Zderzenie: Kosmita -> Gracz (Walka wręcz)
                if (auto* alien = dynamic_cast<Alien*>(entities[i].get())) {
                    if (alien->isDestroyed() || player->isDestroyed()) continue;
                    if (alien->getBounds().intersects(player->getBounds())) {
                        alien->destroy(); // Kosmita ginie przy uderzeniu
                        player->takeDamage(20); // Gracz traci sporo zdrowia
                    }
                }

                // 3. Zderzenie: Kula Wroga -> Gracz (Strzały od wrogów)
                if (auto* alienBullet = dynamic_cast<AlienBullet*>(entities[i].get())) {
                    if (alienBullet->isDestroyed() || player->isDestroyed()) continue;
                    if (alienBullet->getBounds().intersects(player->getBounds())) {
                        alienBullet->destroy();
                        player->takeDamage(10); // Obrażenia od lasera wroga
                    }
                }

                // 4. Zderzenie: Apteczka -> Gracz (Leczenie)
                if (auto* medkit = dynamic_cast<Medkit*>(entities[i].get())) {
                    if (medkit->isDestroyed() || player->isDestroyed()) continue;
                    if (medkit->getBounds().intersects(player->getBounds())) {
                        medkit->destroy(); // Apteczka znika
                        player->heal(30);  // Gracz odzyskuje 30 punktów życia
                    }
                }
            }

            // OPRÓŻNIENIE BUFORA: Teraz, gdy bezpiecznie przejrzeliśmy wektor, wrzucamy do niego nowe obiekty (strzały i dropy)
            for (auto& newEnt : newEntities) {
                entities.push_back(std::move(newEnt));
            }

            // SYSTEM CZYSZCZENIA PAMIĘCI (Zarządzanie wektorem)
            // Znajduje i usuwa z wektora wszystkie obiekty, które zostały oznaczone jako "destroyed" (np. uderzyły w coś lub wyleciały za ekran)
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameObject>& obj) { return obj->isDestroyed(); }), entities.end());

            // Aktualizacja interfejsu tekstowego z bieżącymi statystykami
            hudText.setString("HP: " + std::to_string(player->getHealth()) +
                " | Poziom: " + std::to_string(currentLevel) +
                " | Zlikwidowani: " + std::to_string(killCount));

            // Sprawdzanie warunku przegranej gry
            if (player->getHealth() <= 0) {
                currentState = GAMEOVER;
            }
        }

        // ==========================================
        // --- RENDEROWANIE (Rysowanie na ekranie) ---
        // ==========================================
        window.clear(sf::Color::Black); // Czyszczenie starej klatki

        if (currentState == PLAYING) {
            window.draw(background); // Najpierw rysujemy mapę

            // Następnie rysujemy wszystkie obiekty w grze (Gracz, Wrogowie, Pociski, Apteczki)
            for (auto& entity : entities) {
                entity->draw(window);
            }

            window.draw(hudText); // Na samej górze rysujemy interfejs użytkownika
        }
        else if (currentState == MENU) {
            centerText.setString("ALIEN HARVEST\n\nWcisnij ENTER aby rozpoczac");

            // Matematyczne wyśrodkowanie tekstu względem jego rozmiaru i rozmiaru okna (800x600)
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f);

            window.draw(centerText);
        }
        else if (currentState == GAMEOVER) {
            centerText.setString("GAME OVER!\nZlikwidowani kosmici: " + std::to_string(killCount) + "\n\nWcisnij ENTER aby zagrac ponownie");

            // Matematyczne wyśrodkowanie tekstu
            sf::FloatRect textRect = centerText.getLocalBounds();
            centerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            centerText.setPosition(800.0f / 2.0f, 600.0f / 2.0f);

            window.draw(centerText);
        }

        window.display(); // Wyświetlenie gotowej klatki na monitorze
    }
    return 0; // Prawidłowe zakończenie programu
}