#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager(sf::RenderWindow& win) : window(win)
{
}

void SceneManager::pushScene(std::unique_ptr<Scene> newScene)
{
    scenes.push(std::move(newScene));
}

void SceneManager::popScene()
{
    if (!scenes.empty()) {
        scenes.pop();
    }
}

void SceneManager::changeScene(std::unique_ptr<Scene> newScene)
{
    if (!scenes.empty()) {
        scenes.pop();
    }

    scenes.push(std::move(newScene));
}

void SceneManager::handleEvent(const sf::Event& event)
{
    if (!scenes.empty()) {
        scenes.top()->handleEvent(event, window); 
    }
}

void SceneManager::update(sf::Time dt)
{
    if (!scenes.empty()) {
        scenes.top()->update(dt);
    }
}

void SceneManager::draw()
{
    if (!scenes.empty()) {
        scenes.top()->draw(window);
    }
}

bool SceneManager::isEmpty() const
{
    return scenes.empty();
}