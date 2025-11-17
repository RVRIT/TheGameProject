#include "MainMenu.h"
#include "SceneManager.h"   
#include "SettingsMenu.h" 
#include <iostream>
#include <memory>

MainMenu::MainMenu(SceneManager& manager, sf::RenderWindow& windowRef)
    : sceneManager(manager),
    window(windowRef),
    playButton("assets/play.png", { 700.f, 200.f },
        [this]() {
            // TODO
            // sceneManager.pushScene(std::make_unique<GameScene>(...));
        }),
        exitButton("assets/exit.png", { 700.f, 400.f },
            [this]() {
                sceneManager.popScene(); 
            }),
    settingsButton("assets/settings.png", { 700.f, 1000.f },
        [this]() {
            // TODO
            // sceneManager.pushScene(std::make_unique<SettingsMenu>(sceneManager, window));
        })
{
    bgTexture.loadFromFile("assets/backgroundMainMenu.png");
    background.setTexture(bgTexture);
}

void MainMenu::update(sf::Time dt)
{
}

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    playButton.handleEvent(event, window);
    exitButton.handleEvent(event, window);
    settingsButton.handleEvent(event, window);
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