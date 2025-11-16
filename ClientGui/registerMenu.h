#pragma once
#include "button.h"
#include "textBox.h"
#include "networkClient.h"

class RegisterMenu
{
public:
	RegisterMenu(sf::Font& font, NetworkClient& client, std::function<void()> onClick);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);


private:
	NetworkClient& client;
	Button CreateAccount;
	Button Back;
	TextBox username;
	TextBox password;
	sf::Texture bgTexture;
	sf::Sprite background;

};

