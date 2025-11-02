#pragma once
#include "button.h"
#include "textBox.h"

class RegisterMenu
{
public:
	RegisterMenu(sf::Font& font, std::function<void()> onClick);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

private:
	Button CreateAccount;
	Button Back;
	TextBox username;
	TextBox password;
	TextBox confirmPassword;
	sf::Texture bgTexture;
	sf::Sprite background;

};

