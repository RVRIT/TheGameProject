#include "MainMenu.h"
#include "SceneManager.h"   
#include "SettingsMenu.h" 
#include "GameScene.h" 
#include <memory>


MainMenu::MainMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef)
    : font(fontRef),
    client(clientRef), // <--- FIX: Initialize client
    sceneManager(manager),
    window(windowRef),
    // --- BUTTON INITIALIZERS START ---
    playButton("assets/play.png", { 700.f, 200.f },
        [this]() {
            // Pass client to GameScene (we will fix GameScene later)
           // sceneManager.changeScene(std::make_unique<GameScene>(font, sceneManager, window, client));
            std::cout << "Play clicked\n";
        }), // <--- COMMA HERE IS CRITICAL

    exitButton("assets/exit.png", { 700.f, 400.f },
        [this]() {
            sceneManager.popScene();
        }), // <--- COMMA HERE IS CRITICAL

    settingsButton("assets/settings.png", { 700.f, 600.f },
        [this]() {
            sceneManager.pushScene(std::make_unique<SettingsMenu>(window, sceneManager, font));
        }), // <--- COMMA HERE IS CRITICAL

    createLobbyButton("assets/btn_create_lobby.png", { 700.f, 500.f },
        [this]() {
            std::cout << "Requesting Lobby Creation...\n";
            // Call the client function (make sure createLobby exists in NetworkClient)
            // int id = client.createLobby("PlayerHost"); 
            // if(id != -1) std::cout << "Lobby Created: " << id << "\n";
        })
    // --- BUTTON INITIALIZERS END ---
{
    // Constructor Body
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
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    playButton.draw(window);
    exitButton.draw(window);
    settingsButton.draw(window);
	createLobbyButton.draw(window);
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