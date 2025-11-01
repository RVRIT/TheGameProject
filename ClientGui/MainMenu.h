#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

#define MaxMeinMenu 4

class MainMenu
{
public:
	MainMenu(float width, float height);
	void draw(sf::RenderWindow& window);
	
	int MainMenuPressed() { return MainMenuSelected; }

	~MainMenu();

private:
	int MainMenuSelected;
	sf::Font font;
	
};

