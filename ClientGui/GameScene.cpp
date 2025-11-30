#include "GameScene.h"
#include <iostream>

GameScene::GameScene(sf::Font& f, SceneManager& manager, sf::RenderWindow& window)
    : font(f),
    sceneManager(manager),
    windowRef(window)
{
    std::cout << "GameScene created!\n";
}

void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow&)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        std::cout << "Clicked inside GameScene! testValue = " << testValue << "\n";
    }
}

void GameScene::update(sf::Time)
{
    // nothing yet
}

void GameScene::draw(sf::RenderWindow& window)
{
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(40);
    t.setFillColor(sf::Color::White);

    t.setString("GameScene Working!\nTestValue = " + std::to_string(testValue));
    t.setPosition(100, 100);

    window.draw(t);
}
