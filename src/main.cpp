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
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Title");

    float size = 100.f;
    float groundY = 600.f - size;

    sf::RectangleShape shape({ size, size });
    shape.setPosition({ 100.f, groundY });

    float velocityY = 0.f;
    float gravity = 1500.f;
    float jumpStrength = -600.f;
    bool onGround = true;


    window.setKeyRepeatEnabled(false);
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Space && onGround)
                {
                    velocityY = jumpStrength;
                    onGround = false;
                }
            }
        }

        // Grawitacja
        velocityY += gravity * dt;
        shape.move({ 0.f, velocityY * dt });

        // Kolizja z ziemią
        if (shape.getPosition().y >= groundY)
        {
            shape.setPosition({ shape.getPosition().x, groundY });
            velocityY = 0.f;
            onGround = true;
        }

        window.clear(sf::Color(64, 64, 64));
        window.draw(shape);
        window.display();
    }

    return 0;
}
