#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(sf::RenderWindow& win, SceneManager& mgr, sf::Font& f)
    : window(win), sceneManager(mgr), font(f),
    backButton("assets/back.png", { 50.f, 50.f }, [&]() { mgr.popScene(); }), 
    fullscreenButton("assets/fullscreen.png", { 400.f, 300.f }, []() {}),
    volumeButton("assets/volume.png", { 400.f, 450.f }, []() {})
{
    if (bgTexture.loadFromFile("assets/backgroundSettings.png")) {
        background.setTexture(bgTexture);
    }

    titleText.setFont(font);
    titleText.setString("Settings");
    titleText.setPosition(500, 50);
}

void SettingsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    backButton.handleEvent(event, mousePos);
    fullscreenButton.handleEvent(event, mousePos);
    volumeButton.handleEvent(event, mousePos);
}

void SettingsMenu::update(sf::Time dt) {}

void SettingsMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(titleText);
    backButton.draw(window);
    fullscreenButton.draw(window);
    volumeButton.draw(window);
}