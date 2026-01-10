#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include "TextBox.h" 
#include <SFML/Graphics.hpp>
#include <string>

class LobbyScene : public Scene {
public:
    LobbyScene(sf::Font& font, NetworkClient& client, SceneManager& manager, int lobbyId, std::string playerName);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;
    int lobbyId;

    std::string myName;
    int myPlayerId = -1;
    bool amIReady = false;

    sf::Text titleText;
    sf::Text infoText;

    Button backButton;
    Button readyButton; 

    sf::RectangleShape chatBackground;
    sf::Text chatDisplay;
    TextBox chatInput;
    Button sendButton;

    void parseLobbyState(const std::string& jsonStr);

    Button startGameButton;
    bool isHost = false;
};