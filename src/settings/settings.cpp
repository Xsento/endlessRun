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
    float offsetX = 100.f;
    float posX = 0.f;
    sf::Vector2f pos({100.f, offsetY});
    FloatText playTx("jump", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button playBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);
    FloatText settingsTx("left", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button settingsBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);
    FloatText exitTx("right", font_, defaultTextColor, {200.f, posX += offsetY}, 28);
    Button exitBt("", font_, defaultTextColor, {200.f + offsetX, posX}, 28);

    texts_.push_back(playTx);
    texts_.push_back(settingsTx);
    texts_.push_back(exitTx);

    buttons_.push_back(playBt);
    buttons_.push_back(settingsBt);
    buttons_.push_back(exitBt);
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
