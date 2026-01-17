#pragma once
#include <SFML/Graphics.hpp>

class Card {
public:
    Card(const sf::Texture& texture, sf::Font& font, int value);

    void setPosition(float x, float y); 
    void setPosition(const sf::Vector2f& pos);

    void setScale(float factor); 

    int getValue() const;           
    sf::FloatRect getBounds() const; 

    void draw(sf::RenderWindow& window);

    void select();   
    void deselect();

private:
    sf::Sprite m_sprite;
    sf::Text m_text;
    int m_value;

    void centerText();
};