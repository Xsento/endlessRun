#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <menu/button.hpp>

#include <vector>

namespace menu {

    enum class EventType {
        Mouse,
        Keyboard,
    };

class Menu {
    public:
    Menu(sf::RenderWindow &window);
        void draw();
        void update();
        void checkButton(sf::Vector2f);
        private:
        sf::RenderWindow &window_;
        std::vector<Button> buttons_;
        sf::Font font_;
    };

}
