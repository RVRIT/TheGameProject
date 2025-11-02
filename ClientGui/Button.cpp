#include "Button.h"
#include <iostream>

Button::Button(const std::string& texturePath, sf::Vector2f position, std::function<void()> callback)
    : onClick(callback)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load button texture: " << texturePath << "\n";
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    bool inside = sprite.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

    if (inside && !hovered) {
        hovered = true;
        sprite.setScale(1.1f, 1.1f);
    }
    else if (!inside && hovered) {
        hovered = false;
        sprite.setScale(1.f, 1.f);
    }

    if (inside && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (onClick) onClick();
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
