#include "MainMenu.h"
#include "SceneManager.h"   
#include "SettingsMenu.h" 
#include "GameScene.h" // <--- ADDED: Include GameScene
#include <iostream>
#include <memory>

MainMenu::MainMenu(sf::Font& fontRef, SceneManager& manager, sf::RenderWindow& windowRef)
    : font(fontRef), // <--- ADDED: Initialize font
    sceneManager(manager),
    window(windowRef),
    playButton("assets/play.png", { 700.f, 200.f },
        [this]() {
            // IMPLEMENTED: Change to GameScene passing the font, manager, and window
            sceneManager.changeScene(std::make_unique<GameScene>(font, sceneManager, window));
        }),
    exitButton("assets/exit.png", { 700.f, 400.f },
        [this]() {
            sceneManager.popScene();
        }),
    settingsButton("assets/settings.png", { 700.f, 1000.f },
        [this]() {
            // Note: SettingsMenu is not a Scene class in your code, so pushScene won't work directly here.
            // You likely need to wrap SettingsMenu in a Scene or handle it differently.
            std::cout << "Settings clicked (Implementation pending wrapper)\n";
        })
{
    bgTexture.loadFromFile("assets/backgroundMainMenu.png");
    background.setTexture(bgTexture);
}

void MainMenu::update(sf::Time dt)
{
}

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    playButton.handleEvent(event, mousePos);
    exitButton.handleEvent(event, mousePos);
    settingsButton.handleEvent(event, mousePos);
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    playButton.draw(window);
    exitButton.draw(window);
    settingsButton.draw(window);
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