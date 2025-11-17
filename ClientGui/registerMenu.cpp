#include "registerMenu.h"
#include "SceneManager.h"
#include <iostream>


RegisterMenu::RegisterMenu(sf::Font& font, NetworkClient& clientRef, SceneManager& manager) :
    client(clientRef),
    sceneManager(manager), 
    CreateAccount("assets/CreateAccountButton.png", { 100.f, 500.f }, [this]() {
    std::string usernameText = username.getText();
    std::string passwordText = password.getText();
    if (client.registerUser(usernameText, passwordText)) {
        std::cout << "Register success!\n";
        sceneManager.popScene(); 
    }
    else {
        std::cout << "Register failed!\n";
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
}

void RegisterMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    CreateAccount.handleEvent(event, window);
    Back.handleEvent(event, window);
    username.handleEvent(event);
    password.handleEvent(event);
}

void RegisterMenu::update(sf::Time dt)
{
    // Nimic de actualizat deocamdat?
}

void RegisterMenu::draw(sf::RenderWindow& window)
{
    window.draw(background);
    CreateAccount.draw(window);
    Back.draw(window);
    username.draw(window);
    password.draw(window);
}
