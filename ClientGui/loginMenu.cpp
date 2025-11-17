#include "LoginMenu.h"
#include "SceneManager.h"     
#include "RegisterMenu.h"     
#include "MainMenu.h"         
#include <iostream>
#include <memory>

LoginMenu::LoginMenu(sf::Font& font, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& win) :
	client(clientRef),
	sceneManager(manager),
	window(win),
	LoginButton("assets/LoginButton.png", { 700.f, 400.f }, 
		[this]() {
			std::string usernameText = username.getText();
			std::string passwordText = password.getText();

			if (usernameText.empty() || passwordText.empty()) {
				errorText.setString("Numele si parola nu pot fi goale!");
				return; 
			}

			if (client.loginUser(usernameText, passwordText)) {
				std::cout << "Login success!\n";
				errorText.setString("");
				sceneManager.changeScene(std::make_unique<MainMenu>(sceneManager, window));
			}
			else {
				std::cout << "Login failed!\n";
				errorText.setString("Nume de utilizator sau parola incorecte!");
			}
		}),
	RegisterButton("assets/RegisterButton.png", { 1000.f, 400.f },
		[this, &font]() {
			sceneManager.pushScene(std::make_unique<RegisterMenu>(font, client, sceneManager));
		}),
	username(font, { 700.f, 200.f }, { 500.f, 50.f }),
	password(font, { 700.f, 300.f }, { 500.f, 50.f })
{
	bgTexture.loadFromFile("assets/backgroundTry2.png");
	background.setTexture(bgTexture);

	errorText.setFont(font);
	errorText.setFillColor(sf::Color::Red);
	errorText.setCharacterSize(12); 
	errorText.setPosition(700.f, 500.f);
}


void LoginMenu::update(sf::Time dt)
{
    
}

void LoginMenu::draw(sf::RenderWindow& window)
{
    window.draw(background);
    LoginButton.draw(window);
    RegisterButton.draw(window);
    username.draw(window);
    password.draw(window);
	window.draw(errorText);
}

void LoginMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    LoginButton.handleEvent(event, mousePos);
    RegisterButton.handleEvent(event, mousePos);
    username.handleEvent(event);
    password.handleEvent(event);
	
}