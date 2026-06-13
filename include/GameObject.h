#pragma once
#include <SFML/Graphics.hpp>

/// Główna klasa bazowa (abstrakcyjna) dla wszystkich obiektów w grze.
class GameObject {
protected:
    sf::Vector2f position; // Aktualna pozycja obiektu na ekranie (X, Y)
    float rotation;        // Kąt rotacji obiektu (w stopniach)
    bool destroyed;        // Flaga określająca, czy obiekt "nie żyje" i ma zostać usunięty z wektora

public:
    // Konstruktor ustawiający pozycję początkową i domyślne wartości
    GameObject(float x, float y) : position(x, y), rotation(0.0f), destroyed(false) {}
    // Wirtualny destruktor niezbędny do poprawnego usuwania obiektów klas pochodnych
    virtual ~GameObject() = default;

    // metody czysto wirtualne - kazdy typ (gracz/wrog) musi to nadpisac pod siebie
    virtual void update(float deltaTime) = 0;   // Aktualizacja logiki obiektu (ruch, liczniki)
    virtual void draw(sf::RenderWindow& window) = 0;    // Rysowanie obiektu w oknie gry
    virtual sf::FloatRect getBounds() const = 0; // hitbox pod kolizje

    bool isDestroyed() const { return destroyed; }  // Sprawdza, czy obiekt jest oznaczony do usunięcia
    void destroy() { destroyed = true; }    // Oznacza obiekt jako zniszczony
    sf::Vector2f getPosition() const { return position; }   // Pobiera aktualną pozycję

    // NOWOŚĆ MILESTONE 8: Metoda pozwalająca wymusić zmianę pozycji (np. po zderzeniu z drzewem)
    void setPosition(float x, float y) { position.x = x; position.y = y; }

    float getRotation() const { return rotation; }  // Pobiera aktualny kąt rotacji
};