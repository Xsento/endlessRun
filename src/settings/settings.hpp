#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace controls {
    inline sf::Keyboard::Scancode jump = sf::Keyboard::Scancode::Space;
    inline sf::Keyboard::Scancode left = sf::Keyboard::Scancode::A;
    inline sf::Keyboard::Scancode right = sf::Keyboard::Scancode::D;
}

namespace settings {
    void setDefault();
    void loadFromFile();
    void saveToFile();

    class View {
        public:
        View(sf::RenderWindow &window);
        void draw();
    };
}
