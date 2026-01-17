#pragma once
#include <SFML/Graphics.hpp>

class Card {
public:
    // Constructorul cere textura (poza goal?), fontul ?i num?rul c?r?ii
    Card(const sf::Texture& texture, sf::Font& font, int value);

    // Func?ii de baz?
    void setPosition(float x, float y); // Mut? ?i poza ?i textul
    void setPosition(const sf::Vector2f& pos);

    void setScale(float factor); // Ca s? le faci mai mici/mari

    // Func?ii pentru logica jocului
    int getValue() const;           // Returneaz? num?rul (ex: 99)
    sf::FloatRect getBounds() const; // Ca s? vezi dac? dai click pe ea

    // Desenarea
    void draw(sf::RenderWindow& window);

    // Func?ii vizuale extra (op?ional)
    void select();   // O face pu?in mai luminoas? sau o ridic?
    void deselect(); // O pune la loc

private:
    sf::Sprite m_sprite;
    sf::Text m_text;
    int m_value;

    // Func?ie intern? s? centreze textul perfect pe mijlocul c?r?ii
    void centerText();
};