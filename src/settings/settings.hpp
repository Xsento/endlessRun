#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <menu/button.hpp>
#include <game.hpp>

namespace controls {
    inline sf::Keyboard::Scancode jump = sf::Keyboard::Scancode::Space;
    inline sf::Keyboard::Scancode left = sf::Keyboard::Scancode::A;
    inline sf::Keyboard::Scancode right = sf::Keyboard::Scancode::D;
    inline sf::Keyboard::Scancode pause = sf::Keyboard::Scancode::P;
}

namespace settings {
    void setDefault();
    void loadFromFile();
    void saveToFile();

    class View {
        public:
        View(sf::RenderWindow &window, sf::Font &font);
        void draw();
        int checkButton(sf::Vector2f);
        private:
        sf::RenderWindow &window_;
        std::vector<Button> buttons_;
        std::vector<FloatText> texts_;
        sf::Font &font_;
    };
}
