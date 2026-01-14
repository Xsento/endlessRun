#include <menu/menu.hpp>

using namespace menu;

Menu::Menu(sf::RenderWindow &window)
    : window_(window), font_("assets/fonts/adwaita.ttf") {

    sf::Color textColor(161, 193, 129, 255);
    Button play("Play", font_, textColor, {400.f, 300.f}, 44);
    Button settings("Settings", font_, textColor, {400.f, 400.f}, 44);
    Button exit("Exit", font_, textColor, {400.f, 500.f}, 44);

    buttons_.push_back(play);
    buttons_.push_back(settings);
    buttons_.push_back(exit);
}

void Menu::draw() {
    window_.clear(sf::Color(47, 52, 62, 255));
    for(auto &button : buttons_) {
        button.draw(window_);
    }
}

int Menu::checkButton(sf::Vector2f pos) {
    int iter = 0;
    for (auto& button: buttons_) {
        if (button.onClick(pos)) {
            return iter;
        }
        iter++;
    }
    return -1;
}
