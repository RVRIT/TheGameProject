#include "Card.h"

Card::Card(const sf::Texture& texture, sf::Font& font, int value)
    : m_value(value)
{
    m_sprite.setTexture(texture);

    m_text.setFont(font);
    m_text.setString(std::to_string(value));
    m_text.setCharacterSize(30); 
    m_text.setFillColor(sf::Color::Black); 
    m_text.setStyle(sf::Text::Bold);

    centerText();
}

void Card::centerText() {
    sf::FloatRect textRect = m_text.getLocalBounds();
    sf::FloatRect spriteRect = m_sprite.getGlobalBounds();

    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);


    float xPos = m_sprite.getPosition().x + spriteRect.width / 2.0f;
    float yPos = m_sprite.getPosition().y + (spriteRect.height * 0.25f);

    m_text.setPosition(xPos, yPos);
}

void Card::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
    centerText(); 
}

void Card::setPosition(const sf::Vector2f& pos) {
    setPosition(pos.x, pos.y);
}

void Card::setScale(float factor) {
    m_sprite.setScale(factor, factor);
    m_text.setScale(factor, factor);
    centerText(); 
}

int Card::getValue() const {
    return m_value;
}

sf::FloatRect Card::getBounds() const {
    return m_sprite.getGlobalBounds();
}

void Card::draw(sf::RenderWindow& window) {
    window.draw(m_sprite); 
    window.draw(m_text);   
}

void Card::select() {
    m_sprite.setColor(sf::Color(200, 200, 200)); 
}

void Card::deselect() {
    m_sprite.setColor(sf::Color::White); 
}