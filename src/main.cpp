// system headers
#include <iostream>
#include <math.h>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// project headers
#include <game.hpp>
#include <menu/menu.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Title");
    window.setFramerateLimit(60);
    Game_state state = Game_state::Menu;

    //Rozmiar duszka
    float size = 100.f;

    //Pozycja na ziemi
    float groundY = 600.f - size;

    //Definicja obiektu
    sf::RectangleShape shape({ size, size });
    shape.setPosition({ 100.f, groundY });

    //Predkosc poczatkowa wznoszenia sie
    float velocityY = 0.f;

    //Sila grawitacji
    float gravity = 1500.f;

    //Sila skoku
    float jumpStrength = -600.f;

    //Czy obniekt jest na ziemi
    bool onGround = true;

    //Przyciski nie "spamują"
    window.setKeyRepeatEnabled(false);

    //Start zegarka
    sf::Clock clock;

    while (window.isOpen())
    {

        if (state == Game_state::Menu) {
            menu::Menu menu(window);
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scancode::Enter) {
                        state = Game_state::Running;
                    }
                }
                else if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        switch (menu.checkButton(mousePos)) {
                            case 0:
                                state = Game_state::Running;
                                break;
                            case 1:
                                std::cout << "unimplemented feature" << std::endl;
                                break;
                            case 2:
                                std::cout << "exiting game" << std::endl;
                                return 0;
                        }
                    }
                }
            }

            menu.draw();
            window.display();
        }
        else if (state == Game_state::Running) {
            //Roznica czasu
            float dt = clock.restart().asSeconds();

            while (auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                //Sprawdza czy przycisk jest wcisniety
                else if (auto* key = event->getIf<sf::Event::KeyPressed>())
                {
                    //Sprawdza czy tym przyciskiem jest spacji i czy obiekt jest na ziemi
                    if (key->scancode == sf::Keyboard::Scancode::Space && onGround)
                    {
                        velocityY = jumpStrength;
                        onGround = false;
                    }
                }
            }

            // Grawitacja
            velocityY += gravity * dt;

            //Skok
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
    }

    return 0;
}
