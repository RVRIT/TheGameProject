#include "loginMenu.h"
#include "SceneManager.h"     
#include "RegisterMenu.h"     
#include "MainMenu.h"         
#include <iostream>
#include "json.hpp"
#include <memory>

using json = nlohmann::json;

LoginMenu::LoginMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& win) :
	font(fontRef),
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

			auto response = client.loginUser(usernameText, passwordText);

			if (response.first) {
				std::cout << "LOGIN SUCCESFUL!\n";
				errorText.setString("");
				sceneManager.changeScene(std::make_unique<MainMenu>(font, sceneManager, window));
			}
			else {
				std::cout << "LOGIN FAILED! RAW JSON: " << response.second << '\n';

				try
				{
					json jsonResponse = json::parse(response.second);

					if (jsonResponse.contains("message")) {
						std::string msg = jsonResponse["message"];
						errorText.setString(msg);
					}
					else errorText.setString("Eroare necunoscuta de la server");
				}
				catch (const json::parse_error&) { 
					errorText.setString("Eroare de comunicare cu serverul.");
				}
			}
		}),
	RegisterButton("assets/RegisterButton.png", { 1000.f, 400.f },
		[this]() { 
			sceneManager.pushScene(std::make_unique<RegisterMenu>(font, client, sceneManager));
		}),
	username(fontRef, { 700.f, 200.f }, { 500.f, 50.f }),
	password(fontRef, { 700.f, 300.f }, { 500.f, 50.f })
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