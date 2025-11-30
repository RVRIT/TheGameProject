#pragma once
#include <SFML/Graphics.hpp>
#include "button.h"
#include "Scene.h"  

class SceneManager; 

class MainMenu : public Scene 
{
public:
    MainMenu(sf::Font& font, SceneManager& manager, sf::RenderWindow& window);

    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderWindow& window) override; 

    void updateBackgroundScale();

private:
    sf::Font& font;
    SceneManager& sceneManager; 
    sf::RenderWindow& window;
    Button playButton;
    Button exitButton;
    Button settingsButton;
    sf::Texture bgTexture;
    sf::Sprite background;
};