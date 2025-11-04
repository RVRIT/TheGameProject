#pragma once
#include "button.h"
#include "textBox.h"
#include "networkClient.h"

class LoginMenu
{
public:
	LoginMenu(sf::Font& font,NetworkClient& client, std::function<void()> onClick);
	void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	
private:
	NetworkClient& client;
	Button LoginButton;
	Button RegisterButton;
	TextBox username;
	//TextBox password;
	sf::Texture bgTexture;
	sf::Sprite background;
	
};

