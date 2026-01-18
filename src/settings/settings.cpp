#include <fstream>

#include <json/json.hpp>
#include <settings/settings.hpp>
#include <game.hpp>

using json = nlohmann::json;

void settings::setDefault() {
    controls::jump = sf::Keyboard::Scancode::Space;
    controls::up = sf::Keyboard::Scancode::W;
    controls::down = sf::Keyboard::Scancode::S;
    controls::pause = sf::Keyboard::Scancode::P;

    if (!standingTex.loadFromFile("assets/textures/blond_man_standing.png")
        && !airAndWalk1Tex.loadFromFile("assets/textures/blond_man_running_1.png")
        && !walk2Tex.loadFromFile("assets/textures/blond_man_running_2.png")) {
            perror("file missing");
            exit(-1);
    }
    currSkin = "default";
}

void settings::load_skin(std::string name) {
    currSkin = name;
    std::string file = skins[name];
    if (!standingTex.loadFromFile("assets/textures/" + file + "_standing.png")) {
        perror("file not foun");
        exit(-1);
    }
    if (!airAndWalk1Tex.loadFromFile("assets/textures/" + file + "_running_1.png")) {
        perror("file not foun");
        exit(-1);
    }
    if (!walk2Tex.loadFromFile("assets/textures/" + file + "_running_2.png")) {
        perror("file not foun");
        exit(-1);
    }
}

void settings::saveToFile() {
    json stet;
    stet["controls"]["jump"] =  controls::jump;
    stet["controls"]["up"] =  controls::up;
    stet["controls"]["down"] = controls::down;
    stet["controls"]["pause"] = controls::pause;
    stet["skin"] = currSkin;

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
        controls::up = stt["controls"]["up"];
        controls::down = stt["controls"]["down"];
        controls::pause = stt["controls"]["pause"];

        load_skin(stt["skin"]);
    }
}

settings::View::View(sf::RenderWindow &window, sf::Font &font)
    : window_(window), font_(font) {
    float offsetY = 100.f; // offset dla osi y
    float offsetX = 70.f; // offset dla osi x
    float const mult = 0.7; // mnożnik dla offset
    float posY = 0.f; // pozycja w osi y
    sf::Vector2f pos({100.f, offsetY});
    // utworzenie opisów oraz przycisków
    FloatText controlsTx("controls:", font_, defaultTextColor, {200.f, posY += offsetY}, 28);
    FloatText jumpTx("jump", font_, defaultTextColor, {200.f, posY += offsetY * mult}, 28);
    Button jumpBt("", font_, defaultTextColor, {200.f + offsetX, posY}, 28);
    FloatText upTx("up", font_, defaultTextColor, {200.f, posY += offsetY * mult}, 28);
    Button upBt("", font_, defaultTextColor, {200.f + offsetX, posY}, 28);
    FloatText downTx("down", font_, defaultTextColor, {200.f, posY += offsetY * mult}, 28);
    Button downBt("", font_, defaultTextColor, {200.f + offsetX, posY}, 28);
    FloatText pauseTx("pause", font_, defaultTextColor, {200.f, posY += offsetY * mult}, 28);
    Button pauseBt("", font_, defaultTextColor, {200.f + offsetX, posY}, 28);
    posY = 0;
    FloatText skinTx("skins:", font_, defaultTextColor, {600.f, posY += offsetY}, 28);
    Button skin1Bt("default", font_, defaultTextColor, {600.f, posY += offsetY * mult}, 28);
    Button skin2Bt("black", font_, defaultTextColor, {600.f, posY += offsetY * mult}, 28);
    Button skin3Bt("alex", font_, defaultTextColor, {600.f, posY += offsetY * mult}, 28);
    Button skin4Bt("emo", font_, defaultTextColor, {600.f, posY += offsetY * mult}, 28);
    Button backBt("back", font_, defaultTextColor, {400.f, 550.f}, 28);
    Button resetBt("reset", font_, defaultTextColor, {400.f, 475.f}, 28);
    // dodanie opisów do wektora
    texts_.push_back(controlsTx);
    texts_.push_back(jumpTx);
    texts_.push_back(upTx);
    texts_.push_back(downTx);
    texts_.push_back(pauseTx);
    texts_.push_back(skinTx);
    // dodanie przycisków do wektora
    buttons_.push_back(jumpBt);
    buttons_.push_back(upBt);
    buttons_.push_back(downBt);
    buttons_.push_back(pauseBt);
    buttons_.push_back(backBt);
    buttons_.push_back(skin1Bt);
    buttons_.push_back(skin2Bt);
    buttons_.push_back(skin3Bt);
    buttons_.push_back(skin4Bt);
    buttons_.push_back(resetBt);
}

void settings::View::draw() {
    window_.clear(sf::Color(47, 52, 62, 255));
    sf::Text ctrls(defaultFont);
    ctrls.setString("controls");

    for (int i = 5;i < 9;i++) {
        auto& button = buttons_[i];
        if (button.getString() == currSkin) {
            button.setColor(sf::Color(64,64,64));
        }
        else {
            button.setColor(sf::Color(47, 52, 80));
        }
    }

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
    // przyciemnienie ekranu
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
                        return;
                        case 1:
                        controls::up = key->scancode;
                        return;
                        case 2:
                        controls::down = key->scancode;
                        return;
                        case 3:
                        controls::pause = key->scancode;
                        return;
                        default: break;
                    }
                }
            }
        }
    }
}

void settings::View::changeSkin(int idx) {
    auto& button = buttons_[idx];
    settings::load_skin(button.getString());
}
