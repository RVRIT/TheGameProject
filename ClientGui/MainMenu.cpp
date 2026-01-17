#include "MainMenu.h"
#include "SceneManager.h"    
#include "SettingsMenu.h" 
#include "GameScene.h" 
#include <memory>
#include <iostream> 
#include "LobbyScene.h"
#include <cstdlib> 
#include <ctime>   
#include "GameConstants.h"
#include "LobbyMenuScene.h"
#include "UserStatsScene.h"


MainMenu::MainMenu(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef, std::string username)
    : font(fontRef),
    client(clientRef),
    sceneManager(manager),
    window(windowRef),
    currentUsername(username),

    playButton("assets/play.png",
        { Config::Menu::BUTTON_X, Config::Menu::SLOT_1_Y },
        [this]() {
            sceneManager.pushScene(std::make_unique<LobbyMenuScene>(font, client, sceneManager, window, currentUsername));
        }),

    exitButton("assets/exit.png",
        { Config::Menu::BUTTON_X, Config::Menu::SLOT_4_Y },
        [&]() {
            sceneManager.popScene(); 
            window.close();
        }),

  
    settingsButton("assets/settings.png", { -1000.f, -1000.f }, []() {}),

    statsButton("assets/userStats.png",
        { Config::Menu::BUTTON_X, Config::Menu::SLOT_2_Y },
        [this]() { 
    sceneManager.pushScene(std::make_unique<UserStatsScene>(
        font, client, sceneManager, window, currentUsername
    ));
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
    statsButton.handleEvent(event, mousePos);

}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    playButton.draw(window);
    exitButton.draw(window);
    settingsButton.draw(window);
    statsButton.draw(window);

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