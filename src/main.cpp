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

//window dimensions
unsigned int windowWidth = 800;
unsigned int windowHeight = 600;


int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Title");
    window.setFramerateLimit(60);
    Game_state gameState = Game_state::Menu;
    Camera_state cameraState = Camera_state::Side; // startowa pozycja hamery
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
    // PRZECIWNICY
    // ======================
    float enemySpeed = 250.f;
    std::vector<sf::RectangleShape> enemyVect;
    sf::RectangleShape enemy1({30.f,30.f});
    enemy1.setPosition({(float)windowWidth, groundY+20.f});
    enemyVect.push_back(enemy1);
    float enemySpawnRate = 20; // %
    sf::Time timeSinceLastSpawn = sf::Time::Zero;


    // =====================
    // TORY RUCHU (CAMERA ABOVE)
    // =====================
    std::vector<float> pathPosX = {200, (float)windowWidth/2, (float)windowWidth-200}; // pozycje na osi X torow ruchu (wyciagniete z dupy)
    unsigned short int currentPath = 1;
    const float playerAboveCamOffsetX = 25;
    const float playerAboveCamOffsetY = windowHeight-100;
    bool changePath = false;
    float destinationX; // end position for path changing 
    const float pathChangeSpeed = 450.f;

    // ======================
    // PĘTLA GRY
    // ======================
    while (window.isOpen())
    {
        if (gameState == Game_state::Menu) {
            menu::Menu menu(window);
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scancode::Enter) {
                        gameState = Game_state::Running;
                    }
                }
                else if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        switch (menu.checkButton(mousePos)) {
                            case 0:
                                gameState = Game_state::Running;
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
        else if (gameState == Game_state::Running) {
            float dt = clock.restart().asSeconds();

            // ----------------------
            // EVENTY
            // ----------------------
            if (cameraState == Camera_state::Side) {
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
                        // pauza i powrót do gry
                        if (key->scancode == sf::Keyboard::Scancode::P)
                        {
                            if (gameState == Game_state::Running) {
                                gameState = Game_state::Paused;
                                clock.stop();
                                std::cout<<"paused"<<std::endl;
                            }
                        }
                        // zmiana trybu kamery
                        if (key->scancode == sf::Keyboard::Scancode::C)
                        {
                            enemyVect.clear(); // wypierdalam wszystkich z wektora bo narazie nie wiem co z nimi zrobic
                            cameraState = Camera_state::Above;
                            player.setPosition({pathPosX.at(currentPath) - playerAboveCamOffsetX, playerAboveCamOffsetY});
                        }
                    }
                    if (auto* key = event->getIf<sf::Event::KeyReleased>())
                    {
                        if (gameState == Game_state::Running && key->scancode == sf::Keyboard::Scancode::Space && jumping)
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
                        gameState = Game_state::End;
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
            else {
                // debug path objects, just for showing them visually
                sf::RectangleShape path0({(float)windowHeight,1.f});
                sf::RectangleShape path1({(float)windowHeight,1.f});
                sf::RectangleShape path2({(float)windowHeight,1.f});
                path0.setPosition({pathPosX.at(0), 0.f});
                path1.setPosition({pathPosX.at(1), 0.f});
                path2.setPosition({pathPosX.at(2), 0.f});
                path0.setFillColor(sf::Color(0,255,0,255)); // green
                path1.setFillColor(sf::Color(255,0,0,255)); // red
                path2.setFillColor(sf::Color(0,0,255,255)); // blue
                path0.rotate(sf::degrees(90));
                path1.rotate(sf::degrees(90));
                path2.rotate(sf::degrees(90));

                while (auto event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                        window.close();

                    if (auto* key = event->getIf<sf::Event::KeyPressed>())
                    {
                        // pauza i powrót do gry
                        if (key->scancode == sf::Keyboard::Scancode::P)
                        {
                            if (gameState == Game_state::Running) {
                                gameState = Game_state::Paused;
                                clock.stop();
                                std::cout<<"paused"<<std::endl;
                            }
                        }
                        // zmiana trybu kamery
                        if (key->scancode == sf::Keyboard::Scancode::C)
                        {
                            enemyVect.clear(); // wypierdalam wszystkich z wektora bo narazie nie wiem co z nimi zrobic
                            cameraState = Camera_state::Side;
                            player.setPosition({100.f, groundY});
                        }
                        // zmiana torów ruchu
                        if (key->scancode == sf::Keyboard::Scancode::A)
                        {
                            if (!changePath){
                                currentPath--;
                                if (currentPath > 2) currentPath = 0;
                                std::cout << currentPath << std::endl;
                                destinationX = pathPosX.at(currentPath) - playerAboveCamOffsetX;
                                changePath = true;
                            }
                        }
                        if (key->scancode == sf::Keyboard::Scancode::D)
                        {
                            if (!changePath) {
                                currentPath++;
                                if (currentPath > 2) currentPath = 2;
                                std::cout << currentPath << std::endl;
                                destinationX = pathPosX.at(currentPath) - playerAboveCamOffsetX;
                                changePath = true;
                            }
                        }
                    }
                    if (auto* key = event->getIf<sf::Event::KeyReleased>())
                    {

                    }
                }

                window.clear(sf::Color(64, 64, 64));

                window.draw(path0);
                window.draw(path1);
                window.draw(path2);

                if (changePath){
                    if (abs(player.getPosition().x - destinationX) < 10){
                        player.setPosition({destinationX, playerAboveCamOffsetY});
                        changePath = false;
                    }
                    else {
                        if (destinationX > player.getPosition().x){
                            player.move({pathChangeSpeed*dt,0.f});
                        }
                        else {
                            player.move({-pathChangeSpeed*dt,0.f});
                        }
                    }
                }

                window.draw(player);

                window.display();

            }
        }
        else if (gameState == Game_state::Paused) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if(auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scancode::P) {
                        gameState = Game_state::Running;
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
        else if (gameState == Game_state::End)
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
