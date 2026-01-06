#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "json.hpp"
#include "TextBox.h"

class LobbyScene : public Scene {
public:
    LobbyScene(sf::Font& font, NetworkClient& client, SceneManager& manager, int lobbyId);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;
    int lobbyId;

    sf::Text titleText;
    sf::Text infoText;
    Button backButton;
    
    sf::RectangleShape chatBackground;
    sf::Text chatDisplay;       
    TextBox chatInput;         
    Button sendButton;

    std::string chatHistoryString; 

    void parseLobbyState(const std::string& jsonStr);
};