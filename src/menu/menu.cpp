#include <menu/menu.hpp>

Menu::Menu(sf::RenderWindow &window)
    : window_(window)
{
    if (!this->font_.openFromFile("assets/fonts/adwaita.ttf")) {
        perror("coulnd load font");
    }

    sf::Text play(font_);
    play.setString("PLAY");
    sf::Text settings(font_);
    play.setFillColor(sf::Color(161, 193, 129, 255));
    settings.setString("Settings");
    settings.setFillColor(sf::Color(161, 193, 129, 255));
    sf::Text exit(font_);
    exit.setString("Exit");
    exit.setFillColor(sf::Color(161, 193, 129, 255));

    textst_.push_back(play);
    textst_.push_back(settings);
    textst_.push_back(exit);

    float offset = 350.f;
    for(auto &text : textst_) {
        text.setPosition({300, offset});
        offset+= 75.f;
    }
}

void Menu::draw() {
    window_.clear(sf::Color(47, 52, 62, 255));
    for(auto text : textst_) {
        window_.draw(text);
    }
}
