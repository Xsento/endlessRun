#include <settings/settings.hpp>

void settings::setDefault() {
    controls::jump = sf::Keyboard::Scancode::Space;
    controls::left = sf::Keyboard::Scancode::A;
    controls::right = sf::Keyboard::Scancode::D;
}
