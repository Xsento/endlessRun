#include <game.hpp>

FloatText::FloatText(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f pos, int size)
    :text_(font) {
    text_.setFont(font);
    text_.setCharacterSize(size);
    text_.setFillColor(color);
    text_.setOutlineColor(sf::Color::Black);
    text_.setString(text);
    auto bounds = text_.getLocalBounds();
    rect_ = bounds;
    text_.setOrigin(rect_.getCenter());
    text_.setPosition(pos);
}

void FloatText::draw(sf::RenderWindow &window) {
    window.draw(text_);
}
