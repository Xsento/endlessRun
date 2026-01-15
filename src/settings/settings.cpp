#include <fstream>

#include <json/json.hpp>
#include <settings/settings.hpp>
#include <game.hpp>

using json = nlohmann::json;

void settings::setDefault() {
    controls::jump = sf::Keyboard::Scancode::Space;
    controls::left = sf::Keyboard::Scancode::A;
    controls::right = sf::Keyboard::Scancode::D;
    controls::pause = sf::Keyboard::Scancode::P;

}

void settings::load_skin() {

}

void settings::saveToFile() {
    json stet;
    stet["controls"]["jump"] =  controls::jump;
    stet["controls"]["left"] =  controls::left;
    stet["controls"]["right"] = controls::right;

    std::ofstream file("settings.json");
    file << stet.dump(4);
    file.close();
}

void settings::loadFromFile() {
    skins.insert({"defautl", settings::defaultSkin});
    skins.insert({"skin2", settings::blackSkin});
    skins.insert({"skin3", settings::blondSkin});

    std::ifstream file("settings.json");
    if (!file.is_open()) {
        setDefault();
        return;
    }
    else {
        json stt;
        file >> stt;
        controls::jump = stt["controls"]["jump"];
        controls::left = stt["controls"]["left"];
        controls::right = stt["controls"]["right"];


    }
}


settings::View::View(sf::RenderWindow &window, sf::Font &font)
    : window_(window), font_(font) {
    float offsetY = 100.f;
    float offsetX = 70.f;
    float posX = 0.f;
    sf::Vector2f pos({100.f, offsetY});
    FloatText playTx("jump", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button playBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);
    FloatText settingsTx("left", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button settingsBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);
    FloatText pauseTx("right", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button pauseBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);

    Button backBt("back", font_, defaultTextColor, {400.f, 500.f}, 28);

    texts_.push_back(playTx);
    texts_.push_back(settingsTx);
    texts_.push_back(pauseTx);

    buttons_.push_back(playBt);
    buttons_.push_back(settingsBt);
    buttons_.push_back(pauseBt);
    buttons_.push_back(backBt);
}

void settings::View::draw() {
    window_.clear(sf::Color(47, 52, 62, 255));
    sf::Text ctrls(defaultFont);
    ctrls.setString("controls");

    for(auto &button : buttons_) {
        button.draw(window_);
    }
    for(auto &text : texts_) {
        text.draw(window_);
    }
}

int settings::View::checkButton(sf::Vector2f pos) {
    int iter = 0;
    for (auto& button: buttons_) {
        if (button.onClick(pos)) {
            return iter;
        }
        iter++;
    }
    return -1;
}

void settings::View::changeKey(int btId) {
    sf::RectangleShape bScreen;
    bScreen.setSize({800.f, 600.f});
    bScreen.setFillColor(sf::Color(64, 64, 64, 124));
    FloatText txt("press any key", font_, defaultTextColor, {400.f, 300.f}, 64);
    window_.draw(bScreen);
    txt.draw(window_);
    window_.display();
    while (true) {
        while (auto event = window_.pollEvent()) {
            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    return;
                }
                else {
                    switch (btId) {
                        case 0:
                        controls::jump = key->scancode;
                        return settings::saveToFile();
                        case 1:
                        controls::left = key->scancode;
                        return settings::saveToFile();
                        case 2:
                        controls::right = key->scancode;
                        return settings::saveToFile();
                        case 3:
                        controls::pause = key->scancode;
                        return settings::saveToFile();
                        default: break;
                    }
                }
            }
        }
    }
}
