// system headers
#include <iostream>
#include <math.h>
#include <vector>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// project headers


//window dimensions
unsigned int windowWidth = 800;
unsigned int windowHeight = 600;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Title");
    window.setKeyRepeatEnabled(false);

    // ======================
    // TEKSTURY
    // ======================
    sf::Texture standingTex;
    if (!standingTex.loadFromFile("assets/textures/human_standing.png"))
        return -1;

    sf::Texture airAndWalk1Tex;
    if (!airAndWalk1Tex.loadFromFile("assets/textures/human_walking_1.png"))
        return -1;

    sf::Texture walk2Tex;
    if (!walk2Tex.loadFromFile("assets/textures/human_walking_2.png"))
        return -1;



    // ======================
    // GRACZ
    // ======================
    sf::Sprite player(standingTex);

    // skalowanie (opcjonalnie)
    player.setScale({0.5f, 0.5f});

    float groundY = windowHeight - player.getGlobalBounds().size.y;
    player.setPosition({100.f, groundY});

    // ======================
    // FIZYKA
    // ======================
    float velocityY    = 0.f;
    float gravity      = 1500.f;
    float jumpStrength = -400.f;

    bool onGround = true;
    bool jumping  = false;

    // ======================
    // ANIMACJA
    // ======================
    float animTimer = 0.f;
    float animSpeed = 0.2f;
    int currentFrame = 0;

    sf::Time jumpTime     = sf::Time::Zero;
    sf::Time jumpTimeMax  = sf::seconds(0.2f);

    sf::Clock clock;

    std::vector<sf::Texture*> walkFrames = {
        &standingTex,
        &airAndWalk1Tex,
        &standingTex,
        &walk2Tex
    };


    sf::Clock animClock;
    float frameTime = 0.5f;
    int frame = 0;

    // ======================
    // PĘTLA GRY
    // ======================
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        
        // ----------------------
        // EVENTY
        // ----------------------

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Space && onGround)
                    {
                        // START SKOKU
                        jumping   = true;
                        onGround  = false;
                        velocityY = jumpStrength;
                        jumpTime  = sf::Time::Zero;

                        player.setTexture(airAndWalk1Tex);
                    }
                }
            if (auto* key = event->getIf<sf::Event::KeyReleased>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Space && jumping)
                {
                    // SKOK PRZERWANY WCZEŚNIEJ
                    jumping   = false;
                    jumpTime  = sf::Time::Zero;
                }
            }

            
        }

        // ----------------------
        // LOGIKA SKOKU
        // ----------------------
        if (jumping)
        {
            jumpTime += sf::seconds(dt);
            if (jumpTime >= jumpTimeMax)
            {
                jumping  = false;
                jumpTime = sf::Time::Zero;
            }
        }
        else
        {
            velocityY += gravity * dt;
        }

        player.move({0.f, velocityY * dt});

        // ----------------------
        // KOLIZJA Z ZIEMIĄ
        // ----------------------
        if (player.getPosition().y >= groundY)
        {
            player.setPosition({player.getPosition().x, groundY});
            velocityY = 0.f;

            if (!onGround)
            {
                onGround = true;
                player.setTexture(standingTex);
            }
        }

        if (onGround)
        {
            animTimer += dt;
            if (animTimer >= animSpeed)
            {
                animTimer = 0.f;
                currentFrame = (currentFrame + 1) % walkFrames.size();
                player.setTexture(*walkFrames[currentFrame]);
            }
        }

        // ----------------------
        // RYSOWANIE
        // ----------------------
        window.clear(sf::Color(64, 64, 64));
        window.draw(player);
        window.display();
    }

    return 0;
}
