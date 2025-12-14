#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include "TextBox.h"
#include <vector>

class LobbyScene : public Scene {
public:
    LobbyScene(sf::Font& font, NetworkClient& client, SceneManager& manager);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;

    sf::Text titleText;
    sf::Text playerListText;
    sf::Text chatLogText;

    TextBox chatInput;
    Button sendButton;
    Button readyButton;
    Button backButton;

    sf::Time timeSinceLastUpdate;

	//TO IMPLEMENT 
    void fetchLobbyState();
    void parseLobbyJson(const std::string& jsonStr);
};