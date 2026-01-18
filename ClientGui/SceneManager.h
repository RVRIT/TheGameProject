#pragma once

#include "Scene.h"
#include <stack>
#include <memory> 
class SceneManager
{
public:
    SceneManager(sf::RenderWindow& window);

  
    void pushScene(std::unique_ptr<Scene> newScene);
    void popScene();
    void changeScene(std::unique_ptr<Scene> newScene);
    void handleEvent(const sf::Event& event);
    void update(sf::Time dt);
    void draw();

    bool isEmpty() const;

private:
    std::stack<std::unique_ptr<Scene>> scenes;
    sf::RenderWindow& window;
};

