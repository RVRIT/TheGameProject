#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class SettingsMenu {
public:
    SettingsMenu(sf::RenderWindow& windowRef,
        std::function<void()> onBack,
        std::function<void()> onToggleFullscreen);

    void draw();
    void handleInput(const sf::Event& event);

private:
    sf::RenderWindow& window;

    Button backButton;
    Button fullscreenButton;
    Button volumeButton; // de pus slider

    sf::Texture bgTexture;
    sf::Sprite background;

    std::function<void()> backCallback;
    std::function<void()> fullscreenCallback;
};