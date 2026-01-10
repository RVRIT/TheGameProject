#include "MainMenu.h"
#include "SceneManager.h"    
#include "SettingsMenu.h" 
#include "GameScene.h" 
#include <memory>
#include <iostream> 
#include "LobbyScene.h"
#include <cstdlib> 
#include <ctime>   

MainMenu::MainMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef, std::string username)
    : font(fontRef),
    client(clientRef),
    sceneManager(manager),
    window(windowRef),
    currentUsername(username),

    // 1. PLAY BUTTON
    playButton("assets/play.png", { 50.f, 50.f }, []() {
    std::cout << "Play clicked\n";
        }),

    // 2. EXIT BUTTON
    exitButton("assets/exit.png", { 50.f, 600.f }, [&]() {
    sceneManager.popScene();
        }),

    // 3. SETTINGS BUTTON
    settingsButton("assets/settings.png", { 50.f, 400.f }, [&]() {
    sceneManager.pushScene(std::make_unique<SettingsMenu>(window, sceneManager, font));
        }),

    // 4. CREATE LOBBY BUTTON (Corectat)
    createLobbyButton("assets/btn_create_lobby.png", { 50.f, 150.f }, [this]() {
    std::string hostName = this->currentUsername;
    std::cout << "Requesting Lobby Creation as " << hostName << "...\n";

    int newLobbyId = this->client.createLobby(hostName);

    if (newLobbyId != -1) {
        // Aici aveam nevoie de 'window' la final - este corect acum
        sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, newLobbyId, this->currentUsername, window));
    }
    // AM ȘTERS codul greșit cu 'id' care era aici
        }),

    // 5. INPUT BOX
    lobbyIdInput(font, { 50.f, 300.f }, { 200.f, 40.f }),

    // 6. JOIN LOBBY BUTTON (Corectat)
    joinLobbyButton("assets/btn_join.png", { 270.f, 300.f }, [this]() {
    std::string idStr = lobbyIdInput.getText();
    std::string joinerName = this->currentUsername;

    if (idStr.empty()) return;
    try {
        int id = std::stoi(idStr); // Aici definim 'id'

        if (client.joinLobby(id, joinerName)) {
            // FIX: Am adăugat ', window' la finalul listei de argumente!
            sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id, joinerName, window));
        }
    }
    catch (...) {}
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