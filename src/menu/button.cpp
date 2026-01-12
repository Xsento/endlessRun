#include <menu/button.hpp>

Button::Button(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f pos)
    :text_(font) {
    text_.setFont(font);
    text_.setCharacterSize(44);
    text_.setFillColor(color);
    text_.setOutlineColor(sf::Color::Black);
    text_.setString(text);
    rect_ = text_.getLocalBounds();
    text_.setOrigin(rect_.getCenter());
    text_.setPosition(pos);
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(text_);
}

bool Button::onClick(sf::Vector2f pos) {
    return text_.getGlobalBounds().contains(pos);
}

sf::String Button::getString() {
    return text_.getString();
}
