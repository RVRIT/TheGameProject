#include "LobbyScene.h"

LobbyScene::LobbyScene(sf::Font& font, NetworkClient& client, SceneManager& manager)
    : font(font), client(client), sceneManager(manager),
    chatInput(font, { 700.f, 500.f }, { 500.f, 40.f }),
    sendButton("assets/send.png", { 720.f, 500.f }, [this]() { /* Send Logic Later */ }),
    readyButton("assets/ready.png", { 50.f, 500.f }, [this]() { /* Toggle Ready Later */ }),
    backButton("assets/back.png", { 50.f, 50.f }, [this]() { this->sceneManager.popScene(); })
{
    titleText.setFont(font);
    titleText.setString("Game Lobby");
    titleText.setCharacterSize(40);
    titleText.setPosition(300, 20);

    playerListText.setFont(font);
    playerListText.setString("Players:");
    playerListText.setPosition(50, 100);
    playerListText.setCharacterSize(20);

    chatLogText.setFont(font);
    chatLogText.setString("Chat:");
    chatLogText.setPosition(700, 100);
    chatLogText.setCharacterSize(18);
}

void LobbyScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    chatInput.handleEvent(event);
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sendButton.handleEvent(event, mousePos);
    readyButton.handleEvent(event, mousePos);
    backButton.handleEvent(event, mousePos);
}

void LobbyScene::update(sf::Time dt)
{
   
}

void LobbyScene::draw(sf::RenderWindow& window) {
    // background sprite to create
    window.clear(sf::Color(20, 20, 40)); 

    window.draw(titleText);
    window.draw(playerListText);
    window.draw(chatLogText);

    chatInput.draw(window);
    sendButton.draw(window);
    readyButton.draw(window);
    backButton.draw(window);
}