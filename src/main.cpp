// system headers
#include <iostream>
#include <math.h>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// project headers


//window dimensions
uint windowWidth = 800;
uint windowHeight = 600;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Title");

    //Rozmiar gracza
    float playerSize = 100;

    //Pozycja na ziemi
    float groundY = windowHeight - playerSize;

    //placeholder tekstury
    sf::Texture defaultTex("assets/textures/placeholder.jpeg", false, sf::IntRect({50, 50}, {playerSize, playerSize}));
    sf::Texture flipTex("assets/textures/placeholderFlip.jpeg");
    sf::Texture evilTex("assets/textures/evilPlaceholder.jpeg");
    
    //Definicja obiektu gracza
    sf::Sprite player(defaultTex);
    player.setPosition({ 100.f, groundY });

    //Predkosc poczatkowa wznoszenia sie
    float velocityY = 0.f;

    //Sila grawitacji
    float gravity = 1500.f;

    //Sila skoku
    float jumpStrength = -400.f;

    //Czas skoku i maksymalny czas skoku
    sf::Time jumpTime = sf::seconds(0);
    sf::Time jumpTimeMax = sf::seconds(0.5f);

    //Czy obniekt jest na ziemi
    bool onGround = true;

    //czy gracz skacze
    bool jumping = false;

    //Przyciski nie "spamują"
    window.setKeyRepeatEnabled(false);

    //Start zegarka
    sf::Clock clock;

    while (window.isOpen())
    {
        //Roznica czasu
        float dt = clock.restart().asSeconds();

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            //Sprawdza czy przycisk jest wcisniety
            else if (auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                //Sprawdza czy tym przyciskiem jest spacji i czy obiekt jest na ziemi
                if (key->scancode == sf::Keyboard::Scancode::Space && onGround)
                {
                    jumping = true;
                    velocityY = jumpStrength;
                    onGround = false;
                }
            }
            else if (auto* key = event->getIf<sf::Event::KeyReleased>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Space)
                {
                    jumping = false;
                    jumpTime = sf::seconds(0);
                }
            }

        }

        if (jumping && jumpTime >= jumpTimeMax){
            jumping = false;
            jumpTime = sf::seconds(0);
        }

        // Grawitacja
        if (!jumping) 
            velocityY += gravity * dt;
        else
            jumpTime += sf::seconds(dt);
            std::cout << jumpTime.asSeconds() << std::endl;

        //Skok
        player.move({ 0.f, velocityY * dt });

        // Kolizja z ziemią
        if (player.getPosition().y >= groundY)
        {
            player.setPosition({ player.getPosition().x, groundY });
            velocityY = 0.f;
            onGround = true;
        }

        window.clear(sf::Color(64, 64, 64));
        window.draw(player);
        window.display();
    }

    return 0;
}
