#include "registerMenu.h"
#include "SceneManager.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;


RegisterMenu::RegisterMenu(sf::Font& font, NetworkClient& clientRef, SceneManager& manager) :
    client(clientRef),
    sceneManager(manager),
    CreateAccount("assets/CreateAccountButton.png", { 100.f, 400.f }, [this]() {
    std::string usernameText = username.getText();
    std::string passwordText = password.getText();

    if (usernameText.empty() || passwordText.empty()) {
        errorText.setString("Numele si parola nu pot fi goale!");
        return;
    }

    auto response = client.registerUser(usernameText, passwordText);
    if (response.first) {
        std::cout << "REGISTER SUCCESFUL!\n";
        errorText.setString("");
        sceneManager.popScene();
    }
    else {
        std::cout << "REGISTER FAILED! RAW JSON RESPONSE: " << response.second << '\n';
        try {
            json jsonResponse = json::parse(response.second);

            if (jsonResponse.contains("message")) {
                std::string msg = jsonResponse["message"];
                errorText.setString(msg);
            }
            else {
                errorText.setString("Eroare necunoscuta de la server.");
            }
        }
        catch (const json::parse_error&) { 
            errorText.setString("Eroare interna server (Invalid JSON).");
        }
    }
        }),
    Back("assets/BackButton.png", { 380.f, 400.f }, [this]() {
    sceneManager.popScene();
        }),
    username(font, { 100.f, 200.f }, { 500.f, 50.f }),
    password(font, { 100.f, 300.f }, { 500.f, 50.f }),
    cardAnimation("assets/cardflip.png", 64, 64, 16, 12.f, true)
{
    bgTexture.loadFromFile("assets/background.png");
    background.setTexture(bgTexture);

    errorText.setFont(font);
    errorText.setFillColor(sf::Color::White);
    errorText.setCharacterSize(12);
    errorText.setPosition(100.f, 400.f);

    cardAnimation.setPosition({ 600.f,100.f });
    cardAnimation.setScale(10.f, 10.f);
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
    cardAnimation.update();
}

void RegisterMenu::draw(sf::RenderWindow& window)
{
    window.draw(background);
    CreateAccount.draw(window);
    Back.draw(window);
    username.draw(window);
    password.draw(window);
    window.draw(errorText);
    window.draw(cardAnimation);
}