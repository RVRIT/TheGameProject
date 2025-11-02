#pragma once
#include "button.h"
#include "textBox.h"
class LoginMenu
{
public:
	LoginMenu(sf::Font& font, std::function<void()> onClick);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	
private:
	Button LoginButton;
	Button RegisterButton;
	TextBox username;
	TextBox password;
	sf::Texture bgTexture;
	sf::Sprite background;
};

