#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Button {
    public:
    Button(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f position);
    void draw(sf::RenderWindow&);
    bool onClick(sf::Vector2f);
    sf::String getString();
    private:
    sf::Text text_;
    sf::FloatRect rect_;
};
