#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "Button.h"
#include <SFML/Graphics.hpp>

class SettingsMenu : public Scene
{
public:
    SettingsMenu(sf::RenderWindow& window, SceneManager& manager, sf::Font& font);

    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    sf::RenderWindow& window;
    SceneManager& sceneManager;
    sf::Font& font;

    Button backButton;
    Button fullscreenButton;
    Button volumeButton; 

    sf::Texture bgTexture;
    sf::Sprite background;
    sf::Text titleText;

    sf::Text volumeText;
    sf::RectangleShape sliderBar;      
    sf::RectangleShape sliderFill;     
    sf::CircleShape sliderHandle;      

    bool isDragging = false;           
    float currentVolume = 50.0f;       

    void updateSliderVisuals();

    
};