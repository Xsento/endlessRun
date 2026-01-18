#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

inline const unsigned int windowWidth = 800;
inline const unsigned int windowHeight = 600;

inline sf::Font defaultFont("assets/fonts/adwaita.ttf");
inline sf::Color defaultTextColor(161, 193, 129, 255);
inline sf::Color buttonColor(47, 52, 80, 255);


enum class Game_state {
    Menu,
    Settings,
    Running,
    Paused,
    End,
};

/// podstawowa klasa do wyświetlania tekstu
class FloatText {
    public:
    /// `text`: tekst wyświetlany na przycisku
    /// `&font`: font
    /// `color`: kolor tła przycisku
    /// `position`: pozycja przycisku (wycentrowana)
    /// `size`: rozmiar czcionki
    FloatText(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f position, int size);
    /// zapisuje tekst do buforu
    void draw(sf::RenderWindow&);
    private:
    sf::Text text_;
    sf::FloatRect rect_;
};
enum class Camera_state {
    Side,
    Above
};
