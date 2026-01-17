#include "Card.h"

Card::Card(const sf::Texture& texture, sf::Font& font, int value)
    : m_value(value)
{
    // 1. Setup Sprite (Poza de fundal)
    m_sprite.setTexture(texture);

    // 2. Setup Text (Numărul)
    m_text.setFont(font);
    m_text.setString(std::to_string(value));
    m_text.setCharacterSize(30); // Ajustează mărimea asta după preferință!
    m_text.setFillColor(sf::Color::Black); // Sau White, depinde de poza ta
    m_text.setStyle(sf::Text::Bold);

    // 3. Centrarea inițială
    centerText();
}

// În Card.cpp

void Card::centerText() {
    // 1. Aflăm dimensiunile pentru text și carte
    sf::FloatRect textRect = m_text.getLocalBounds();
    sf::FloatRect spriteRect = m_sprite.getGlobalBounds();

    // 2. Setăm originea textului în centrul LUI (asta rămâne la fel)
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);

    // 3. POZIȚIONAREA PE CARTE (Aici modificăm)

    // X (Orizontal): Rămâne la mijlocul cărții
    float xPos = m_sprite.getPosition().x + spriteRect.width / 2.0f;

    // Y (Vertical): Îl punem mai sus!
    // Folosim 0.25f (25% din înălțime) ca să fie în partea de sus
    // Dacă vrei și mai sus, scade numărul (ex: 0.20f). Dacă vrei mai jos, crește-l (ex: 0.30f).
    float yPos = m_sprite.getPosition().y + (spriteRect.height * 0.25f);

    m_text.setPosition(xPos, yPos);
}

void Card::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
    centerText(); // Recalculăm poziția textului imediat
}

void Card::setPosition(const sf::Vector2f& pos) {
    setPosition(pos.x, pos.y);
}

void Card::setScale(float factor) {
    m_sprite.setScale(factor, factor);
    m_text.setScale(factor, factor);
    centerText(); // Re-centrăm pentru că s-au schimbat dimensiunile
}

int Card::getValue() const {
    return m_value;
}

sf::FloatRect Card::getBounds() const {
    return m_sprite.getGlobalBounds();
}

void Card::draw(sf::RenderWindow& window) {
    window.draw(m_sprite); // Desenăm întâi fundalul
    window.draw(m_text);   // Apoi numărul peste el
}

// Opțional: Visual Feedback
void Card::select() {
    m_sprite.setColor(sf::Color(200, 200, 200)); // O facem puțin gri când e selectată
}

void Card::deselect() {
    m_sprite.setColor(sf::Color::White); // Reset la normal
}