#include "MainMenu.h"
#include "SceneManager.h"   
#include "SettingsMenu.h" 
#include "GameScene.h" 
#include <memory>
#include "LobbyScene.h"


MainMenu::MainMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef)
    : font(fontRef),
    client(clientRef), 
    sceneManager(manager),
    window(windowRef),
    playButton("assets/play.png", { 700.f, 200.f },
        [this]() {
           // sceneManager.changeScene(std::make_unique<GameScene>(font, sceneManager, window, client));
            std::cout << "Play clicked\n";
        }), 

    exitButton("assets/exit.png", { 700.f, 400.f },
        [this]() {
            sceneManager.popScene();
        }), 

    settingsButton("assets/settings.png", { 700.f, 600.f },
        [this]() {
            sceneManager.pushScene(std::make_unique<SettingsMenu>(window, sceneManager, font));
        }), 

    createLobbyButton("assets/btn_create_lobby.png", { 700.f, 500.f },
        [this]() {
            std::cout << "Requesting Lobby Creation...\n";
            int newLobbyId = this->client.createLobby("Player1"); // We will use real names later

            if (newLobbyId != -1) {
                std::cout << "Success! Entering Lobby " << newLobbyId << "\n";
                sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, newLobbyId));
            }
            else {
                std::cout << "Failed to create lobby. Is Server running?\n";
            }
        }),
    lobbyIdInput(font, { 700.f, 650.f }, { 200.f, 40.f }),
    joinLobbyButton("assets/btn_join.png", { 500.f, 650.f }, [this]() {
    std::string idStr = lobbyIdInput.getText();
    if (idStr.empty()) return;

    try {
        int id = std::stoi(idStr); 
        std::cout << "Joining Lobby " << id << "...\n";

        if (client.joinLobby(id, "Player2")) { // Hardcoded name for now
            std::cout << "Join Successful!\n";
            sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id));
        }
        else {
            std::cout << "Failed to join lobby.\n";
        }
    }
    catch (...) {
        std::cout << "Invalid Lobby ID format.\n";
    }
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