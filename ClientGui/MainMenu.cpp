#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& windowRef)
    : window(windowRef)
{
    if (!playTexture.loadFromFile("assets/play.png"))
        std::cerr << "Failed to load play.png\n";
    if (!exitTexture.loadFromFile("assets/exit.png"))
        std::cerr << "Failed to load exit.png\n";

    playButton.setTexture(playTexture);
    exitButton.setTexture(exitTexture);

    playButton.setPosition(500.f, 250.f);
    exitButton.setPosition(500.f, 400.f);

    
    playButton.setScale(0.5f, 0.5f);
    exitButton.setScale(0.5f, 0.5f);
}

void MainMenu::draw() {
    window.draw(playButton);
    window.draw(exitButton);
}

void MainMenu::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(
            static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y)
        );
    }
}
