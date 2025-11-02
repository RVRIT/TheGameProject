#include "LoginMenu.h"

LoginMenu::LoginMenu(sf::Font& font, std::function<void()> onClick):
	LoginButton("assets/LoginButton.png", { 400.f, 400.f }, []() {} ),
	RegisterButton("assets/RegisterButton.png", { 700.f, 400.f }, onClick),
	username(font, { 400.f, 200.f }, { 500.f, 50.f }),
	password(font, { 400.f, 300.f }, { 500.f, 50.f })
{
	bgTexture.loadFromFile("assets/background.png");
	background.setTexture(bgTexture);
}

void LoginMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	LoginButton.handleEvent(event, window);
	RegisterButton.handleEvent(event, window);
	username.handleEvent(event);
	password.handleEvent(event);
}

void LoginMenu::draw(sf::RenderWindow& window)
{
	window.draw(background);
	LoginButton.draw(window);
	RegisterButton.draw(window);
	username.draw(window);
	password.draw(window);
}
