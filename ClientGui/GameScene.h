#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct VisualCard {
    int value;
    sf::FloatRect bounds; // Zona de click
    bool isSelected;
};

struct VisualPile {
    int topValue;
    int type; // 0 = Crescător (1->99), 1 = Descrescător (100->2)
    sf::FloatRect bounds;
};

struct VisualOpponent {
    std::string name;
    int cardCount;
    bool isTurn;
};

class GameScene : public Scene {
public:
    GameScene(sf::Font& font, NetworkClient& client, SceneManager& manager, int lobbyId, std::string playerName, sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;
    int lobbyId;
    std::string myName;

    // UI
    sf::Text statusText;
    sf::Text deckInfoText;
    Button endTurnButton;

    // Datele jocului
    std::vector<VisualCard> myHand;
    std::vector<VisualPile> piles;
    std::vector<VisualOpponent> opponents;

    int selectedHandIndex = -1; // -1 = nicio carte selectată

    // Helpers
    void parseGameState(const std::string& jsonStr);
    void drawCard(sf::RenderWindow& window, int value, sf::Vector2f pos, bool isSelected, bool isPile, int pileType = -1);

    Button backButton;  
    bool isGameOver = false; 

    sf::RenderWindow& window;
};