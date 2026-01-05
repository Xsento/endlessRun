// system headers
#include <iostream>
#include <math.h>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// project headers

int main()
{
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!");
    window.setFramerateLimit(60);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Texture texture("assets/placeholder.jpeg");
    sf::Sprite sprite(texture);
    sprite.setPosition({300.f, 300.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
                sprite.setPosition({static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y)});
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}
