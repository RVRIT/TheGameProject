#include "TextBox.h"

TextBox::TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color(240, 240, 240));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2.f);

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 5, position.y + 5);
}

void TextBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouse(
            static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y)
        );
        selected = box.getGlobalBounds().contains(mouse);
        box.setOutlineColor(selected ? sf::Color::Red : sf::Color::Black);
    }

    if (selected && event.type == sf::Event::TextEntered) {
        char typed = static_cast<char>(event.text.unicode);

        if (typed == 8 && !input.empty()) { 
            input.pop_back();
        }
        else input += typed;
        
        text.setString(input);
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

void TextBox::clear() {
    this->text.setString("");
}

std::string TextBox::getText() const {
    return input;
}

void TextBox::setSelected(bool sel) {
    selected = sel;
}