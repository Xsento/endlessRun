// system headers
#include <iostream>
#include <math.h>
#include <vector>
#include <random>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// project headers
#include <game.hpp>
#include <menu/menu.hpp>
#include <settings/settings.hpp>


auto getRandomSeed()
    -> std::seed_seq
{
    std::random_device source;
    unsigned int random_data[10];
    for(auto& elem : random_data) {
        elem = source();
    }
    return std::seed_seq(random_data + 0, random_data + 10);
}

double randomnumber() {
    static auto seed = getRandomSeed();
    static std::default_random_engine rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    return dist(rng);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Title");
    window.setFramerateLimit(60);
    window.setMaximumSize(sf::Vector2u{windowWidth, windowHeight});
    window.setMinimumSize(sf::Vector2u{windowWidth, windowHeight});
    Game_state state = Game_state::Menu;
    window.setKeyRepeatEnabled(false);

    // ======================
    // ustawienia
    // ======================
    settings::loadFromFile();



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
    // PRZECIWNICY
    // ======================
    float enemySpeed = 250.f;
    std::vector<sf::RectangleShape> enemyVect;
    sf::RectangleShape enemy1({30.f,30.f});
    enemy1.setPosition({(float)windowWidth, groundY+20.f});
    enemyVect.push_back(enemy1);
    float enemySpawnRate = 20; // %
    sf::Time timeSinceLastSpawn = sf::Time::Zero;

    // ======================
    // PĘTLA GRY
    // ======================
    while (window.isOpen())
    {
        if (state == Game_state::Menu) {
            static menu::Menu menu(window);
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
                        // obsługa przycisków
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        switch (menu.checkButton(mousePos)) {
                            case 0:
                                state = Game_state::Running;
                                break;
                            case 1:
                                state = Game_state::Settings;
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
        else if (state == Game_state::Settings) {
            static settings::View st_viev(window, defaultFont);

            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        // obsługa przycisków
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        switch (st_viev.checkButton(mousePos)) {
                            case 0:
                                st_viev.changeKey(0);
                                break;
                            case 1:
                                st_viev.changeKey(1);
                                break;
                            case 2:
                                st_viev.changeKey(2);
                                break;
                            case 3:
                                st_viev.changeKey(3);
                                break;
                            case 4:
                                settings::saveToFile();
                                state = Game_state::Menu;
                                break;
                            case 5:
                                st_viev.changeSkin(5);
                                break;
                            case 6:
                                st_viev.changeSkin(6);
                                break;
                            case 7:
                                st_viev.changeSkin(7);
                                break;
                            case 8:
                                st_viev.changeSkin(8);
                                break;
                            case 9:
                                settings::setDefault();
                                break;
                        }
                    }
                }
            }

            st_viev.draw();
            window.display();
        }
        else if (state == Game_state::Running) {
            float dt = clock.restart().asSeconds();
            std::cout << dt << std::endl;

            // ----------------------
            // EVENTY
            // ----------------------
            while (auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if (auto* key = event->getIf<sf::Event::KeyPressed>())
                {
                    if (key->scancode == controls::jump && onGround)
                    {
                            // START SKOKU
                            jumping   = true;
                            onGround  = false;
                            velocityY = jumpStrength;
                            jumpTime  = sf::Time::Zero;

                            player.setTexture(airAndWalk1Tex);
                    }
                    // pauza i powrót do gry
                    if (key->scancode == controls::pause)
                    {
                        if (state == Game_state::Running) {
                            state = Game_state::Paused;
                            clock.stop();
                            std::cout<<"paused"<<std::endl;
                        }
                    }
                }
                if (auto* key = event->getIf<sf::Event::KeyReleased>())
                {
                    if (state == Game_state::Running && key->scancode == controls::jump && jumping)
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
            // PRZECIWICY
            // ----------------------

            // spawn nowych przeciwników
            timeSinceLastSpawn += sf::seconds(dt);
            //std::cout << timeSinceLastSpawn.asSeconds() << std::endl;
            if (timeSinceLastSpawn.asMilliseconds() > 1000.f){
                if ((!enemyVect.empty() && randomnumber() < enemySpawnRate) || enemyVect.empty()){
                    sf::RectangleShape newEnemy({30.f,30.f});
                    newEnemy.setPosition({(float)windowWidth, groundY+20.f});
                    enemyVect.push_back(newEnemy);
                }
                timeSinceLastSpawn = sf::Time::Zero;
            }

            // ruch przeciwników
            for (auto& enemy : enemyVect){
                enemy.move(sf::Vector2f({-enemySpeed*dt, 0.f}));
                if (auto collision = player.getGlobalBounds().findIntersection(enemy.getGlobalBounds())){
                    state = Game_state::End;
                }
            }

            // usuwanie przeciwników poza ekranem
            if (!enemyVect.empty() && enemyVect.front().getPosition().x < 0.f){
                enemyVect.erase(enemyVect.begin());
            }

            // skalowanie trudności w czasie
            //enemySpeed += 5.f * dt;
            enemySpawnRate += 0.1f * dt;
            // tmp com
            // std::cout << enemySpawnRate << std::endl;

            // ----------------------
            // RYSOWANIE
            // ----------------------



            window.clear(sf::Color(64, 64, 64));

            window.draw(player);
            for (const auto& enemy : enemyVect){
                window.draw(enemy);
            }

            window.display();
        }
        else if (state == Game_state::Paused) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if(auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == controls::pause) {
                        state = Game_state::Running;
                        clock.start();
                    }
                }
            }
            window.clear(sf::Color(64, 64, 64));

            window.draw(player);
            for (const auto& enemy : enemyVect){
                window.draw(enemy);
            }

            static sf::RectangleShape pauseScreen ({(float)windowWidth, (float)windowHeight});
            pauseScreen.setFillColor(sf::Color(0,0,0,150));
            window.draw(pauseScreen);
            window.display();
        }
        else if (state == Game_state::End)
        {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }

            // ekran game over
            window.clear(sf::Color(64, 64, 64));

            static sf::RectangleShape blackScreen ({(float)windowWidth, (float)windowHeight});
            blackScreen.setFillColor(sf::Color(0,0,0,150));
            window.draw(blackScreen);

            window.display();

        }
    }



    return 0;
}
