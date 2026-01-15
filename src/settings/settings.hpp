#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>

#include <menu/button.hpp>
#include <game.hpp>

namespace controls {
    inline sf::Keyboard::Scancode jump = sf::Keyboard::Scancode::Space;
    inline sf::Keyboard::Scancode left = sf::Keyboard::Scancode::A;
    inline sf::Keyboard::Scancode right = sf::Keyboard::Scancode::D;
    inline sf::Keyboard::Scancode pause = sf::Keyboard::Scancode::P;
}

inline sf::Texture standingTex;
inline sf::Texture airAndWalk1Tex;
inline sf::Texture walk2Tex;
inline std::pmr::unordered_map<std::string, void(*)()> skins;

namespace settings {
    void load_skin();
    void setDefault();
    void loadFromFile();
    void saveToFile();
    void defaultSkin();
    void blackSkin();
    void blondSkin();

    class View {
        public:
        View(sf::RenderWindow &window, sf::Font &font);
        void draw();
        int checkButton(sf::Vector2f);
        void changeKey(int key);
        private:
        sf::RenderWindow &window_;
        std::vector<Button> buttons_;
        std::vector<FloatText> texts_;
        sf::Font &font_;
    };
}
