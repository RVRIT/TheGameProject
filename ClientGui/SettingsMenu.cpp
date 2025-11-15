#include "SettingsMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu(sf::RenderWindow& windowRef,std::function<void()> onBack, std::function<void()> onToggleFullscreen)
    : window(windowRef),
    backCallback(onBack),
    fullscreenCallback(onToggleFullscreen),

    backButton("assets/back.png", { 100.f, 600.f }, [&]() {
    backCallback(); 
        }),

    fullscreenButton("assets/fullscreen.png", { 700.f, 200.f }, [&]() {
    fullscreenCallback(); 
        }),

    volumeButton("assets/volume.png", { 700.f, 350.f }, [&]() {})
{
    bgTexture.loadFromFile("assets/backgroundSettings.png");
    background.setTexture(bgTexture);

    float scaleX = static_cast<float>(window.getSize().x) / bgTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / bgTexture.getSize().y;
    background.setScale(scaleX, scaleY);
}

void SettingsMenu::draw() {
    window.clear();
    window.draw(background);
    backButton.draw(window);
    fullscreenButton.draw(window);
    volumeButton.draw(window);
}

void SettingsMenu::handleInput(const sf::Event& event) {
    backButton.handleEvent(event, window);
    fullscreenButton.handleEvent(event, window);
    volumeButton.handleEvent(event, window);
}

void SettingsMenu::updateBackgroundScale() {
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();

    background.setScale(
        float(win.x) / tex.x,
        float(win.y) / tex.y
    );
}