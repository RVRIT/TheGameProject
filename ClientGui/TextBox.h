#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class TextBox
{
public:
	TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size);
	void handleEvent(const sf::Event& event);
	void draw(sf::RenderWindow& window);
	std::string getText() const { return input; }
	void setSelected(bool sel) { selected = sel; }
	void clear();

private:
	sf::RectangleShape box;
	sf::Text text;
	std::string input;
	bool selected = false;
};

