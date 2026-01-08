#include "MainMenu.h"
#include "SceneManager.h"    
#include "SettingsMenu.h" 
#include "GameScene.h" 
#include <memory>
#include <iostream> 
#include "LobbyScene.h"

MainMenu::MainMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef)
    : font(fontRef),
    client(clientRef),
    sceneManager(manager),
    window(windowRef),

    playButton("assets/play.png", { 50.f, 50.f },
        [this]() {
            std::cout << "Play clicked\n";
        }),

    createLobbyButton("assets/btn_create_lobby.png", { 50.f, 150.f },
        [this]() {
            std::string hostName = "Player1";
            std::cout << "Requesting Lobby Creation...\n";
            int newLobbyId = this->client.createLobby(hostName);

            if (newLobbyId != -1) {
                std::cout << "Success! Entering Lobby " << newLobbyId << "\n";
                sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, newLobbyId, hostName));
            }
            else {
                std::cout << "Failed to create lobby.\n";
            }
        }),

    lobbyIdInput(font, { 50.f, 300.f }, { 200.f, 40.f }),

    joinLobbyButton("assets/btn_join.png", { 270.f, 300.f }, [this]() {
    std::string idStr = lobbyIdInput.getText();
    std::string joinerName = "Player2";

    if (idStr.empty()) return;
    try {
        int id = std::stoi(idStr);
        if (client.joinLobby(id, joinerName)) {
            sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id, joinerName));
        }
    }
    catch (...) {}
        }),

    settingsButton("assets/settings.png", { 50.f, 400.f },
        [this]() {
            sceneManager.pushScene(std::make_unique<SettingsMenu>(window, sceneManager, font));
        }),

    exitButton("assets/exit.png", { 50.f, 600.f },
        [this]() {
            sceneManager.popScene();
        })

{
    if (bgTexture.loadFromFile("assets/backgroundMainMenu.png")) {
        background.setTexture(bgTexture);
        updateBackgroundScale();
    }
}

void MainMenu::update(sf::Time dt)
{
}

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    playButton.handleEvent(event, mousePos);
    exitButton.handleEvent(event, mousePos);
    settingsButton.handleEvent(event, mousePos);
    createLobbyButton.handleEvent(event, mousePos);
    lobbyIdInput.handleEvent(event);
    joinLobbyButton.handleEvent(event, mousePos);
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    playButton.draw(window);
    exitButton.draw(window);
    settingsButton.draw(window);
    createLobbyButton.draw(window);
    lobbyIdInput.draw(window);
    joinLobbyButton.draw(window);
}

void MainMenu::updateBackgroundScale()
{
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();

    background.setScale(
        float(win.x) / tex.x,
        float(win.y) / tex.y
    );
}