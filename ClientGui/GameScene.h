#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"
#include "Button.h"
#include <memory>
#include <vector>
#include <string>

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

    std::unique_ptr<Game> m_game;
    GameSnapshot currentSnapshot;
    std::string playerName = "Player1";

    sf::Text statusText;
    sf::Text logText;
    sf::Text deckCountText;
    sf::Text turnInfoText;
    sf::RectangleShape logBackground;
    Button endTurnButton;

    int selectedHandIndex = -1;
    int hoveredHandIndex = -1;
    int hoveredPileIndex = -1;

    std::vector<std::string> gameLogs;
    sf::Cursor standardCursor;
    sf::Cursor handCursor;

    void refreshSnapshot();
    void logAction(const std::string& msg);
    void updateHUD();
    void updateCursor(sf::RenderWindow& window);
    bool isMoveValid(int handIdx, int pileIdx);

    sf::FloatRect getPileBounds(int pileIndex) const;
    sf::FloatRect getHandCardBounds(int cardIndex, int totalCards) const;
};