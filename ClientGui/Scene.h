#pragma once
#include <SFML/Graphics.hpp>

class SceneManager;

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};