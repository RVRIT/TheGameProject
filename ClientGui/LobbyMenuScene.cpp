#include "LobbyMenuScene.h"
#include "LobbyScene.h"     
#include "GameConstants.h"
#include <iostream>

LobbyMenuScene::LobbyMenuScene(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef, std::string username)
    : font(fontRef), client(clientRef), sceneManager(manager), window(windowRef), currentUsername(username),

    createLobbyBtn("assets/btn_create_lobby.png", { 100.f, 200.f }, [this]() {
    std::cout << "Requesting Create Lobby...\n";
    int newId = this->client.createLobby(this->currentUsername);
    if (newId != -1) {
        sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, newId, this->currentUsername, window));
    }
        }),

    lobbyIdInput(font, { 100.f, 350.f }, { 200.f, 60.f }),

    joinLobbyBtn("assets/btn_join.png", { 100.f, 420.f }, [this]() {
    std::string idStr = lobbyIdInput.getText();
    if (!idStr.empty()) {
        try {
            int id = std::stoi(idStr);
            if (client.joinLobby(id, this->currentUsername)) {
                sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id, this->currentUsername, window));
            }
        }
        catch (...) { std::cout << "Invalid ID format\n"; }
    }
        }),

    backButton("assets/back.png", { 50.f, 800.f }, [this]() {
    sceneManager.popScene(); 
        })
{

    if (bgTexture.loadFromFile("assets/LobbySceneBackground.png")) {
        background.setTexture(bgTexture);
        updateBackgroundScale();
    }



}

void LobbyMenuScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    createLobbyBtn.handleEvent(event, mousePos);
    joinLobbyBtn.handleEvent(event, mousePos);
    backButton.handleEvent(event, mousePos);

    lobbyIdInput.handleEvent(event); 
}

void LobbyMenuScene::update(sf::Time dt) {
    // Aici vei face poll la server să iei lista de lobby-uri (o dată la 2-3 secunde)
}

void LobbyMenuScene::draw(sf::RenderWindow& window) {
    window.draw(background);

    createLobbyBtn.draw(window);
    lobbyIdInput.draw(window);
    joinLobbyBtn.draw(window);
    backButton.draw(window);

    window.draw(listBackground);
    window.draw(listTitle);
}

void LobbyMenuScene::refreshLobbyList() {
    // TODO: Aici vei implementa GET /lobbies când primești endpoint-ul
}

void LobbyMenuScene::updateBackgroundScale()
{
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();

    background.setScale(
        float(win.x) / tex.x,
        float(win.y) / tex.y
    );
}