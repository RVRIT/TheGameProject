#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "SceneManager.h"

class GameScene : public Scene
{
public:
    GameScene(sf::Font& font, SceneManager& manager, sf::RenderWindow& window);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font& font;
    SceneManager& sceneManager;
    sf::RenderWindow& windowRef;

    int testValue = 123;
};
