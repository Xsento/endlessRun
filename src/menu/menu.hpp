#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class Menu {
    public:
    Menu(sf::RenderWindow &window);
    void draw();
    void update();
    void eventHandler();
    private:
    sf::RenderWindow &window_;
    std::vector<sf::Text> textst_;
    sf::Font font_;
};
