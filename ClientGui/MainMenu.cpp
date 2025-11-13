#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& windowRef, std::function<void()> onClick)
    : window(windowRef),
    onSettingsClick(onClick),
    playButton("assets/play.png", { 700.f, 200.f }, [&]()
        {}),
    exitButton("assets/exit.png", { 700.f, 400.f }, [&]() {
    window.close();
        }),
    settingsButton("assets/settings.png", { 700.f, 1000.f }, [&]() {
    onSettingsClick();
      })
{
    bgTexture.loadFromFile("assets/backgroundMainMenu.png");
    background.setTexture(bgTexture);
}

void MainMenu::draw() {
	window.draw(background);
	playButton.draw(window);
	exitButton.draw(window);
    settingsButton.draw(window);
}

void MainMenu::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    playButton.handleEvent(event, window);
    exitButton.handleEvent(event, window);
	settingsButton.handleEvent(event, window);
}
