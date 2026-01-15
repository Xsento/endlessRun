#include <menu/button.hpp>
#include <game.hpp>

Button::Button(sf::String text, sf::Font &font, sf::Color color, sf::Vector2f pos, int size)
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

    const float paddingX = 20.f;
    const float paddingY = 10.f;

    // Prostokąt
    box_.setSize({
        bounds.size.x + paddingX * 2.f,
        bounds.size.y + paddingY * 2.f
    });

    box_.setFillColor(buttonColor);
    box_.setOutlineThickness(4.f);
    box_.setOutlineColor(sf::Color::Black);

    // Wyśrodkowanie
    box_.setOrigin(box_.getSize() / 2.f);
    box_.setPosition(pos);
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(box_);
    window.draw(text_);
}

bool Button::onClick(sf::Vector2f pos) {
    return box_.getGlobalBounds().contains(pos);
}

sf::String Button::getString() {
    return text_.getString();
}

float Button::getSizeX() {
    return rect_.size.x;
}

void Button::setColor(sf::Color color) {
    this->box_.setFillColor(color);
}
