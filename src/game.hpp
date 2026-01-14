#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

inline sf::Font defaultFont("assets/fonts/adwaita.ttf");
inline sf::Color defaultTextColor(161, 193, 129, 255);
inline sf::Color fillColor(47, 52, 62, 255);
inline sf::Color buttonColor(47, 52, 80, 255);

enum class Game_state {
    Menu,
    Settings,
    Running,
    Paused,
    End,
};

class FloatText {
    public:
    FloatText(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f position, int size);
    void draw(sf::RenderWindow&);
    private:
    sf::Text text_;
    sf::FloatRect rect_;
};
