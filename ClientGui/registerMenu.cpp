#include "registerMenu.h"
#include <iostream>


RegisterMenu::RegisterMenu(sf::Font& font, NetworkClient& clientRef, std::function<void()> onClick) :
	client(clientRef),
	CreateAccount("assets/CreateAccountButton.png", { 100.f, 500.f }, [this]() {
	std::string usernameText = username.getText();
	std::string passwordText = password.getText();
	if (client.registerUser(usernameText, passwordText)){
		std::cout << "Register success!\n";
	}
	else {
		std::cout << "Register failed!\n";
	}

		}),
	Back("assets/BackButton.png", { 450.f, 500.f }, onClick),
	username(font, { 100.f, 200.f }, { 500.f, 50.f }),
	password(font, { 100.f, 300.f }, { 500.f, 50.f })
{
	bgTexture.loadFromFile("assets/background.png");
	background.setTexture(bgTexture);
}


void RegisterMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	CreateAccount.handleEvent(event, window);
	Back.handleEvent(event, window);
	username.handleEvent(event);
	password.handleEvent(event);
	//confirmPassword.handleEvent(event);
}

void RegisterMenu::draw(sf::RenderWindow& window)
{
	window.draw(background);
	CreateAccount.draw(window);
	Back.draw(window);
	username.draw(window);
	password.draw(window);
	//confirmPassword.draw(window);
}
