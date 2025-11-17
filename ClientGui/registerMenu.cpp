#include "registerMenu.h"
#include "SceneManager.h"
#include <iostream>


RegisterMenu::RegisterMenu(sf::Font& font, NetworkClient& clientRef, SceneManager& manager) :
    client(clientRef),
    sceneManager(manager), 
    CreateAccount("assets/CreateAccountButton.png", { 100.f, 500.f }, [this]() {
    std::string usernameText = username.getText();
    std::string passwordText = password.getText();

    if(usernameText.empty() || passwordText.empty()) {
		errorText.setString("Numele si parola nu pot fi goale!");
        return; 
	}
    if (client.registerUser(usernameText, passwordText)) {
        std::cout << "Register success!\n";
        sceneManager.popScene(); 
    }
    else {
        std::cout << "Register failed!\n";
		errorText.setString("Numele de utilizator exista deja!");
    }
        }),
    Back("assets/BackButton.png", { 450.f, 500.f }, [this]() {
    sceneManager.popScene();
        }),
    username(font, { 100.f, 200.f }, { 500.f, 50.f }),
    password(font, { 100.f, 300.f }, { 500.f, 50.f })
{
    bgTexture.loadFromFile("assets/background.png");
    background.setTexture(bgTexture);
    
	errorText.setFont(font);
	errorText.setFillColor(sf::Color::White);
	errorText.setCharacterSize(12);
	errorText.setPosition(100.f, 400.f);
}

void RegisterMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    CreateAccount.handleEvent(event, mousePos);
    Back.handleEvent(event, mousePos);
    username.handleEvent(event);
    password.handleEvent(event);
}

void RegisterMenu::update(sf::Time dt)
{
    // Nimic de actualizat deocamdata
}

void RegisterMenu::draw(sf::RenderWindow& window)
{
    window.draw(background);
    CreateAccount.draw(window);
    Back.draw(window);
    username.draw(window);
    password.draw(window);
	window.draw(errorText);
}
