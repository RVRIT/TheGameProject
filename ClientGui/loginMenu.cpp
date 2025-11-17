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

			if (client.loginUser(usernameText, passwordText)) {
				std::cout << "Login success!\n";
				sceneManager.changeScene(std::make_unique<MainMenu>(sceneManager, window));
			}
			else {
				std::cout << "Login failed!\n";
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
}

void LoginMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    LoginButton.handleEvent(event, window);
    RegisterButton.handleEvent(event, window);
    username.handleEvent(event);
    password.handleEvent(event);
}