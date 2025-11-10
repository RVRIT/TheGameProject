#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window);
    void draw();
    void handleInput(const sf::Event& event, sf::RenderWindow& window);

private:
    sf::RenderWindow& window;
    sf::Texture playTexture;
    sf::Texture exitTexture;
    sf::Sprite playButton;
    sf::Sprite exitButton;
};
