#pragma once
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include "Card.h" 
#include "TextBox.h"
#include <vector>
#include <memory> 

struct VisualPile {
    int type; // 0 = ASC, 1 = DESC
    std::unique_ptr<Card> card; 
};

struct VisualOpponent {
    std::string name;
    int cardCount;
    bool isTurn;
};

class GameScene : public Scene {
public:
    GameScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name, sf::RenderWindow& win);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
    void updateBackgroundScale();


private:
    void parseGameState(const std::string& jsonStr);

    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;
    sf::RenderWindow& window;

    int lobbyId;
    std::string myName;
    bool isGameOver = false;

    sf::Sprite background;
    sf::Texture bgTexture;

    sf::Text statusText;
    sf::Text deckInfoText;
    Button endTurnButton;
    Button backButton;

    sf::Texture cardTexture;

    std::vector<std::unique_ptr<Card>> myHand;

    std::vector<VisualPile> piles;

    std::vector<VisualOpponent> players;
    int selectedHandIndex = -1;

    sf::Shader bgShader;
    sf::RectangleShape shaderRect; 
    sf::Clock shaderTimer;
    bool shaderLoaded = false;

    sf::Text cardsPlayedText;

    bool isMenuOpen = false;           
    sf::RectangleShape menuOverlay;    
    sf::RectangleShape menuContainer;  
    sf::Text menuTitle;
    Button btnExit;

    sf::RectangleShape chatBg;    
    TextBox chatInput;              
    std::vector<sf::Text> chatHistory; 

    void updateChatVisuals(const std::vector<std::string>& rawMessages);
    void pollChat();
};