#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"       
#include "Button.h"   
#include <memory>

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

    std::unique_ptr<Game> game;
    GameSnapshot currentSnapshot;
    std::string playerName = "Player1"; // Hardcoded for single player now

    sf::Text statusText; 
    void refreshSnapshot();
    sf::FloatRect getPileBounds(int pileIndex) const;

    int selectedHandIndex = -1;
    sf::FloatRect getHandCardBounds(int cardIndex, int totalCards) const;
};