#include "LoginMenu.h"
#include <iostream>

LoginMenu::LoginMenu(sf::Font& font,NetworkClient& clientRef, std::function<void()> onClick, std::function<void()> onLoginSuccess) :
	client(clientRef),
	LoginButton("assets/LoginButton.png", { 700.f, 400.f }, [this, onLoginSuccess]() {
	std::string usernameText = username.getText();
	if (client.loginUser(usernameText)) {
		std::cout << "Login success!\n";
		onLoginSuccess();
	}
	else {
		std::cout << "Login failed!\n";
	}
		}),
	RegisterButton("assets/RegisterButton.png", { 1000.f, 400.f }, onClick),
	username(font, { 700.f, 200.f }, { 500.f, 50.f })
	//,password(font, { 700.f, 300.f }, { 500.f, 50.f })
{
	bgTexture.loadFromFile("assets/backgroundTry2.png");
	background.setTexture(bgTexture);
}

void LoginMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	LoginButton.handleEvent(event, window);
	RegisterButton.handleEvent(event, window);
	username.handleEvent(event);
	//password.handleEvent(event);
}

void LoginMenu::draw(sf::RenderWindow& window)
{
	window.draw(background);
	LoginButton.draw(window);
	RegisterButton.draw(window);
	username.draw(window);
	//password.draw(window);
}