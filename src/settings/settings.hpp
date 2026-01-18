#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>

#include <menu/button.hpp>
#include <game.hpp>

namespace controls {
    /// przycisk skoku
    inline sf::Keyboard::Scancode jump = sf::Keyboard::Scancode::Space;
    /// przycisk ruchu w lewo
    inline sf::Keyboard::Scancode up = sf::Keyboard::Scancode::W;
    /// przycisk ruchu w prawo
    inline sf::Keyboard::Scancode down = sf::Keyboard::Scancode::D;
    /// przycisk pauzy
    inline sf::Keyboard::Scancode pause = sf::Keyboard::Scancode::P;
}

inline sf::Texture standingTex("assets/textures/blond_man_standing.png");
inline sf::Texture airAndWalk1Tex("assets/textures/blond_man_running_1.png");
inline sf::Texture walk2Tex("assets/textures/blond_man_running_2.png");
inline std::string currSkin = "default";
inline std::pmr::unordered_map<std::string, std::string> skins = {
    {"default", "blond_man"},
    {"black", "black_man"},
    {"alex", "alex_woman"},
    {"emo", "emo_man"},
};

namespace settings {
    /// załadowanie skina dla gracza
    /// `name:` nazwa skina
    void load_skin(std::string name);
    /// przywrócenie ustawień domyślnych
    void setDefault();
    /// załadowanie ustawień z pliku `settings.json`
    void loadFromFile();
    /// zapisanie ustawień do pliku `settings.json`
    void saveToFile();

    class View {
        public:
        View(sf::RenderWindow &window, sf::Font &font);
        void draw();
        int checkButton(sf::Vector2f);
        void changeKey(int key);
        void changeSkin(int button);
        private:
        sf::RenderWindow &window_;
        std::vector<Button> buttons_;
        std::vector<FloatText> texts_;
        sf::Font &font_;
    };
}
