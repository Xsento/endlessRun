// system headers
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Mouse.hpp"
#include "menu/button.hpp"
#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <fstream>
// lib headers
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
// project headers
#include <game.hpp>
#include <menu/menu.hpp>
#include <settings/settings.hpp>
#include <json/json.hpp>


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

double randomnumber(double a, double b) {
    static auto seed = getRandomSeed();
    static std::default_random_engine rng(seed);
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Title");
    window.setFramerateLimit(60);
    Game_state gameState = Game_state::Menu;
    Camera_state cameraState = Camera_state::Side; // startowa pozycja hamery
    window.setMaximumSize(sf::Vector2u{windowWidth, windowHeight});
    window.setMinimumSize(sf::Vector2u{windowWidth, windowHeight});
    window.setKeyRepeatEnabled(false);

    // ======================
    // ustawienia
    // ======================
    settings::loadFromFile();

    // ======================
    // TLO
    // ======================
    sf::Texture backgroundTex;
    if (!backgroundTex.loadFromFile("assets/textures/tlo1.png")) {
        std::cerr << "Error loading background texture" << std::endl;
    }

    sf::Sprite background(backgroundTex);

 
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
    std::vector<sf::Sprite> enemyVect;
    //sf::RectangleShape enemy1({30.f,30.f});
    //enemy1.setPosition({(float)windowWidth, groundY+20.f});
    //enemyVect.push_back(enemy1);
    float enemySpawnRate = 20; // %
    float aCamEnemySpawnRate = 80;
    sf::Time timeSinceLastEnemySpawn = sf::Time::Zero;
    std::vector<sf::Texture> enemyTexturesSide;
    std::vector<sf::Texture> enemyTexturesAbove;
    sf::Texture bench_front, bench_top, plant_top, trashcan_top, trashcan;
    bench_front.loadFromFile("assets/textures/bench_front.png");
    bench_top.loadFromFile("assets/textures/bench_top.png");
    plant_top.loadFromFile("assets/textures/plant_top.png");
    trashcan_top.loadFromFile("assets/textures/trashcan_top.png");
    trashcan.loadFromFile("assets/textures/trashcan.png");
    enemyTexturesSide.push_back(bench_front);
    enemyTexturesSide.push_back(trashcan);
    enemyTexturesAbove.push_back(bench_top);
    enemyTexturesAbove.push_back(plant_top);
    enemyTexturesAbove.push_back(trashcan_top);

    // ======================
    // MODYFIKATORY
    // ======================
    float buffSpeed = 200.f;
    std::vector<sf::Sprite> buffVect;
    float buffSpawnRate = 15; // %
    sf::Time timeSinceLastBuffSpawn = sf::Time::Zero;
    sf::Texture buff1Tex;
    buff1Tex.loadFromFile("assets/textures/monster.png");
    sf::Sprite buff1(buff1Tex);

    // ======================
    // PUNKTY
    // ======================
    int score = 0;
    int multiplier = 1;
    int multiplierDuration = 300;
    sf::Font points;
    points.openFromFile("assets/fonts/Minecraft.ttf");

    sf::Text scoreText(points);

    scoreText.setString(sf::String("Score: ") + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition({10.f, 10.f});

    // ======================
    // ODCZYT I ZAPIS HIGHSCORE
    // ======================
    sf::Text highscoreText(points);
    int highscore;
    std::ifstream highscoreFile("assets/scores/highscore.txt");
    if (highscoreFile.is_open()) {
        highscoreFile >> highscore;
        highscoreFile.close();
    }
    else {
        highscore = 0;
    }

    highscoreText.setString(sf::String("Highscore: ") + std::to_string(highscore));
    highscoreText.setCharacterSize(24);
    highscoreText.setFillColor(sf::Color::Black);
    highscoreText.setPosition({10.f, 40.f});



    // =====================
    // TORY RUCHU (CAMERA ABOVE)
    // =====================
    std::vector<float> pathPosY = {100, (float)windowHeight/2, (float)windowHeight-100}; // pozycje na osi Y torow ruchu (wyciagniete z dupy)
    unsigned short int currentPath = 1;
    const float playerAboveCamOffsetY = 25;
    const float playerAboveCamOffsetX = 100;
    bool changePath = false;
    float destinationY; // end position for path changing
    const float pathChangeSpeed = 550.f;

    // ======================
    // PĘTLA GRY
    // ======================
    while (window.isOpen())
    {
        if (gameState == Game_state::Menu) {
            static menu::Menu menu(window);
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
                        // obsługa przycisków
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        switch (menu.checkButton(mousePos)) {
                            case 0:
                                gameState = Game_state::Running;
                                break;
                            case 1:
                                gameState = Game_state::Settings;
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
        else if (gameState == Game_state::Settings) {
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
                                gameState = Game_state::Menu;
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
        else if (gameState == Game_state::Running) {
            float dt = clock.restart().asSeconds();
            score += 1*multiplier;
            // music.play();
            scoreText.setString(sf::String("Score: ") + std::to_string(score));
            // ----------------------
            // EVENTY
            // ----------------------
            if (cameraState == Camera_state::Side) {
                while (auto event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                        window.close();

                    if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
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
                            player.setPosition({playerAboveCamOffsetX, pathPosY.at(currentPath) - playerAboveCamOffsetY});
                        }
                    }
                    if (auto* key = event->getIf<sf::Event::KeyReleased>())
                    {
                        if (gameState == Game_state::Running && jumping)
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
                timeSinceLastEnemySpawn += sf::seconds(dt);
                if (timeSinceLastEnemySpawn.asMilliseconds() > 1000.f){
                    if ((!enemyVect.empty() && randomnumber(0,100) < enemySpawnRate) || enemyVect.empty()){
                        int textureNo = round(randomnumber(0,enemyTexturesSide.size()-1));
                        sf::Sprite newEnemy(enemyTexturesSide.at(textureNo));
                        newEnemy.setPosition({(float)windowWidth, groundY});
                        enemyVect.push_back(newEnemy);
                    }
                    timeSinceLastEnemySpawn = sf::Time::Zero;
                }

                // ruch przeciwników
                for (auto& enemy : enemyVect){
                    enemy.move(sf::Vector2f({-enemySpeed*dt, 0.f}));
                    if (auto collision = player.getGlobalBounds().findIntersection(enemy.getGlobalBounds())){
                        if(score > highscore){
                            highscore = score;
                            // zapis nowego highscore do pliku
                            std::ofstream highscoreFileOut("assets/scores/highscore.txt");
                            if (highscoreFileOut.is_open()) {
                                highscoreFileOut << highscore;
                                highscoreFileOut.close();
                            }
                        }
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
                // MODYFIKATORY
                // ----------------------
                timeSinceLastBuffSpawn += sf::seconds(dt);
                //std::cout << timeSinceLastSpawn.asSeconds() << std::endl;
                if (timeSinceLastBuffSpawn.asMilliseconds() > 5000.f){
                    if ((!buffVect.empty() && randomnumber(1,100) < buffSpawnRate) || enemyVect.empty()){
                        sf::Sprite newBuff(buff1Tex);
                        newBuff.setPosition({(float)windowWidth, groundY - buff1.getGlobalBounds().size.y + 5.f});
                        buffVect.push_back(newBuff);
                    }
                    timeSinceLastBuffSpawn = sf::Time::Zero;
                }

                // ruch modyfikatorów
                for (auto& buff : buffVect){
                    buff.move(sf::Vector2f({-buffSpeed*dt, 0.f}));
                    if (auto collision = player.getGlobalBounds().findIntersection(buff.getGlobalBounds())){
                        buffVect.erase(buffVect.begin());
                        for(int i=0; i<multiplierDuration; i++){
                            multiplier = 5;
                            
                        }
                        
                    }
                }

                // usuwanie modyfikatorów poza ekranem
                if (!buffVect.empty() && buffVect.front().getPosition().x < 0.f){
                    buffVect.erase(buffVect.begin());
                }

                background.move({-20.f * dt, 0.f});

                // ----------------------
                // RYSOWANIE
                // ----------------------

                window.draw(background);
                window.draw(scoreText);
                window.draw(highscoreText);
                window.draw(player);

                for (const auto& enemy : enemyVect){
                    window.draw(enemy);
                }

                for (const auto& buff : buffVect){
                    window.draw(buff);
                }

                window.display();
            }

            // ===================
            // ABOVE CAMERA MODE
            // ===================
            else {
                // debug path objects, just for displaying them visually
                sf::RectangleShape path0({(float)windowWidth,1.f});
                sf::RectangleShape path1({(float)windowWidth,1.f});
                sf::RectangleShape path2({(float)windowWidth,1.f});
                path0.setPosition({0.f,pathPosY.at(0)});
                path1.setPosition({0.f,pathPosY.at(1)});
                path2.setPosition({0.f,pathPosY.at(2)});
                path0.setFillColor(sf::Color(0,255,0,255)); // green
                path1.setFillColor(sf::Color(255,0,0,255)); // red
                path2.setFillColor(sf::Color(0,0,255,255)); // blue
                //path0.rotate(sf::degrees(90));
                //path1.rotate(sf::degrees(90));
                //path2.rotate(sf::degrees(90));

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
                        if (key->scancode == controls::up)
                        {
                            if (!changePath){
                                currentPath--;
                                if (currentPath > 2) currentPath = 0;
                                destinationY = pathPosY.at(currentPath) - playerAboveCamOffsetY;
                                changePath = true;
                            }
                        }
                        if (key->scancode == controls::down)
                        {
                            if (!changePath) {
                                currentPath++;
                                if (currentPath > 2) currentPath = 2;
                                destinationY = pathPosY.at(currentPath) - playerAboveCamOffsetY;
                                changePath = true;
                            }
                        }
                    }
                    if (auto* key = event->getIf<sf::Event::KeyReleased>())
                    {

                    }
                }

                if (changePath){
                    if (abs(player.getPosition().y - destinationY) < 10){
                        player.setPosition({playerAboveCamOffsetX, destinationY});
                        changePath = false;
                    }
                    else {
                        if (destinationY > player.getPosition().y){
                            player.move({0.f,pathChangeSpeed*dt});
                        }
                        else {
                            player.move({0.f,-pathChangeSpeed*dt});
                        }
                    }
                }

                animTimer += dt;
                    if (animTimer >= animSpeed)
                    {
                        animTimer = 0.f;
                        currentFrame = (currentFrame + 1) % walkFrames.size();
                        player.setTexture(*walkFrames[currentFrame]);
                    }

                // ----------------------
                // PRZECIWICY
                // ----------------------

                // spawn nowych przeciwników
                timeSinceLastEnemySpawn += sf::seconds(dt);
                //std::cout << timeSinceLastSpawn.asSeconds() << std::endl;
                if (timeSinceLastEnemySpawn.asMilliseconds() > 300.f){
                    if ((!enemyVect.empty() && randomnumber(0,100) < aCamEnemySpawnRate) || enemyVect.empty()){
                        int spawnPath = round(randomnumber(0,2));
                        int textureNo = randomnumber(0,enemyTexturesAbove.size()-1);
                        sf::Sprite newEnemy(enemyTexturesAbove.at(textureNo));
                        newEnemy.setPosition({(float)windowWidth, groundY+20.f});
                        enemyVect.push_back(newEnemy);
                    }
                    timeSinceLastEnemySpawn = sf::Time::Zero;
                }

                // ruch przeciwników
                for (auto& enemy : enemyVect){
                    enemy.move(sf::Vector2f({-enemySpeed*dt, 0.f}));
                    if (auto collision = player.getGlobalBounds().findIntersection(enemy.getGlobalBounds())){
                        if(score > highscore){
                            highscore = score;
                            // zapis nowego highscore do pliku
                            std::ofstream highscoreFileOut("assets/scores/highscore.txt");
                            if (highscoreFileOut.is_open()) {
                                highscoreFileOut << highscore;
                                highscoreFileOut.close();
                            }
                        }
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
                // MODYFIKATORY
                // ----------------------
                timeSinceLastBuffSpawn += sf::seconds(dt);
                //std::cout << timeSinceLastSpawn.asSeconds() << std::endl;
                if (timeSinceLastBuffSpawn.asMilliseconds() > 5000.f){
                    if ((!buffVect.empty() && randomnumber(1,100) < buffSpawnRate) || enemyVect.empty()){
                        sf::Sprite newBuff(buff1Tex);
                        newBuff.setPosition({(float)windowWidth, groundY - buff1.getGlobalBounds().size.y + 5.f});
                        buffVect.push_back(newBuff);
                    }
                    timeSinceLastBuffSpawn = sf::Time::Zero;
                }

                // ruch modyfikatorów
                for (auto& buff : buffVect){
                    buff.move(sf::Vector2f({-buffSpeed*dt, 0.f}));
                    if (auto collision = player.getGlobalBounds().findIntersection(buff.getGlobalBounds())){
                        buffVect.erase(buffVect.begin());
                        for(int i=0; i<multiplierDuration; i++){
                            multiplier = 5;
                            
                        }
                        
                    }
                }

                // usuwanie modyfikatorów poza ekranem
                if (!buffVect.empty() && buffVect.front().getPosition().x < 0.f){
                    buffVect.erase(buffVect.begin());
                }

                background.move({-20.f * dt, 0.f});

                // ----------------------
                // RYSOWANIE
                // ----------------------
                window.draw(background);
                window.draw(scoreText);
                window.draw(highscoreText);
                window.draw(player);

                for (const auto& enemy : enemyVect){
                    window.draw(enemy);
                }

                for (const auto& buff : buffVect){
                    window.draw(buff);
                }

                window.draw(path0);
                window.draw(path1);
                window.draw(path2);

                window.display();
            }
        }
        else if (gameState == Game_state::Paused) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if(auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == controls::pause) {
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
            Button playBt("Play again", defaultFont, defaultTextColor, {400, 300}, 44);
            Button menuBt("Main menu", defaultFont, defaultTextColor, {400, 400}, 44);

            score = 0;
            enemyVect.clear();
            buffVect.clear();

            // music.stop();
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                        if (playBt.onClick(mousePos)) {
                            gameState = Game_state::Running;
                        }
                        else if (menuBt.onClick(mousePos)) {
                            gameState = Game_state::Menu;
                        }
                    }
                }
            }

            // ekran game over
            window.clear(sf::Color(64, 64, 64));
            static sf::RectangleShape blackScreen ({(float)windowWidth, (float)windowHeight});
            blackScreen.setFillColor(sf::Color(0,0,0,150));
            window.draw(blackScreen);
            playBt.draw(window);
            menuBt.draw(window);
            window.display();
        }
    }



    return 0;
}
