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

// deklaracja stanów gry
enum GAME_STATE {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAMEOVER
};

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
    window.setKeyRepeatEnabled(false);

    // zmienna przechowująca stan gry
    enum GAME_STATE GAME = GAME_STATE_PLAYING;

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
                if (GAME == GAME_STATE_PLAYING && key->scancode == sf::Keyboard::Scancode::Space && onGround)
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
                    if (GAME == GAME_STATE_PLAYING)
                        GAME = GAME_STATE_PAUSED;
                    else if (GAME == GAME_STATE_PAUSED)
                        GAME = GAME_STATE_PLAYING;
                }
            }
            if (auto* key = event->getIf<sf::Event::KeyReleased>())
            {
                if (GAME == GAME_STATE_PLAYING && key->scancode == sf::Keyboard::Scancode::Space && jumping)
                {
                    // SKOK PRZERWANY WCZEŚNIEJ
                    jumping   = false;
                    jumpTime  = sf::Time::Zero;
                }
            }          
        }

        if (GAME == GAME_STATE_PLAYING){
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
                    GAME = GAME_STATE_GAMEOVER;
                }
                
            }

            // usuwanie przeciwników poza ekranem
            if (!enemyVect.empty() && enemyVect.front().getPosition().x < 0.f){
                enemyVect.erase(enemyVect.begin());
            }

            // skalowanie trudności w czasie
            //enemySpeed += 5.f * dt;
            enemySpawnRate += 0.1f * dt;
            std::cout << enemySpawnRate << std::endl;
        }


        // ----------------------
        // RYSOWANIE
        // ----------------------
        window.clear(sf::Color(64, 64, 64));

        if (GAME == GAME_STATE_PLAYING || GAME == GAME_STATE_PAUSED){
            window.draw(player);
            for (const auto& enemy : enemyVect){
                window.draw(enemy);
            }
            // ekran pauzy
            if (GAME == GAME_STATE_PAUSED){
                static sf::RectangleShape pauseScreen ({(float)windowWidth, (float)windowHeight});
                pauseScreen.setFillColor(sf::Color(0,0,0,150));
                window.draw(pauseScreen);
            }
        }
        // ekran game over
        else if (GAME == GAME_STATE_GAMEOVER){
            static sf::RectangleShape blackScreen ({(float)windowWidth, (float)windowHeight});
            blackScreen.setFillColor(sf::Color(0,0,0,150));
            window.draw(blackScreen);
        }

        window.display();
    }

    return 0;
}
