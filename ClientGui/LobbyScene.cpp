#include "LobbyScene.h"

LobbyScene::LobbyScene(sf::Font& font, NetworkClient& client, SceneManager& manager)
    : font(font), client(client), sceneManager(manager),
    chatInput(font, { 400.f, 500.f }, { 300.f, 40.f }),
    sendButton("assets/send.png", { 720.f, 500.f }, []() {}),
    readyButton("assets/ready.png", { 50.f, 500.f }, []() {}),
    backButton("assets/back.png", { 50.f, 50.f }, [this]() { this->sceneManager.popScene(); })
{
}

void LobbyScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    backButton.handleEvent(event, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void LobbyScene::update(sf::Time dt) {
}

void LobbyScene::draw(sf::RenderWindow& window) {
    backButton.draw(window);
}