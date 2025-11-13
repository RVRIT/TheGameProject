#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "button.h"
#include <string>

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window, std::function<void()> onClick);
    void draw();
    void handleInput(const sf::Event& event, sf::RenderWindow& window);

private:
    sf::RenderWindow& window;
    Button playButton;
	Button exitButton;
    Button settingsButton;
    sf::Texture bgTexture;
    sf::Sprite background;
    std::function<void()> onSettingsClick;
};
