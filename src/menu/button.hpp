#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/// podstawowa implementacja przycisku
class Button {
    public:
    /// `text`: tekst wyświetlany na przycisku
    /// `&font`: font
    /// `color`: kolor tła przycisku
    /// `position`: pozycja przycisku (wycentrowana)
    /// `size`: rozmiar czcionki
    Button(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f position, int size);
    /// zapisanie przycisku do buforu
    void draw(sf::RenderWindow&);
    /// sprawdzanie kolizji z wektorem
    bool onClick(sf::Vector2f position);
    /// return text of a button
    sf::String getString();
    /// return X size of text
    float getSizeX();
    /// ustawienie koloru tła
    void setColor(sf::Color);
    private:
    sf::Text text_;
    sf::FloatRect rect_;
    sf::RectangleShape box_;
};
