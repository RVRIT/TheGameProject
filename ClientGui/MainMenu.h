#pragma once
#include <SFML/Graphics.hpp>
#include "button.h"
#include "Scene.h"  // <-- 1. Include Scene.h

class SceneManager; // <-- 2. Forward-declare SceneManager

class MainMenu : public Scene // <-- 3. Mo?tene?te public din Scene
{
public:
    // 4. Constructorul prime?te SceneManager, nu callback-uri
    MainMenu(SceneManager& manager, sf::RenderWindow& window);

    // 5. Implementeaz? func?iile virtuale din Scene
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderWindow& window) override; // <-- Numele corect

    void updateBackgroundScale();

private:
    SceneManager& sceneManager; // <-- 6. Stocheaz? referin?a la manager
    sf::RenderWindow& window;
    Button playButton;
    Button exitButton;
    Button settingsButton;
    sf::Texture bgTexture;
    sf::Sprite background;
};