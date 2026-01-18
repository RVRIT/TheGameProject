#pragma once
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include "textBox.h"
#include <SFML/Graphics.hpp>   
#include "json.hpp"

class LobbyMenuScene : public Scene {
public:
    LobbyMenuScene(sf::Font& font, NetworkClient& client, SceneManager& manager, sf::RenderWindow& window, std::string username);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
    void updateBackgroundScale();

private:
    sf::Font& font;
    NetworkClient& client;
    SceneManager& sceneManager;
    sf::RenderWindow& window;
    std::string currentUsername;

    sf::Sprite background;
    sf::Texture bgTexture;

    Button createLobbyBtn;

    TextBox lobbyIdInput;
    Button joinLobbyBtn;

    Button backButton;

    sf::RectangleShape listBackground;
    sf::Text listTitle;

    nlohmann::json lobbiesData;

    float refreshTimer = 0.0f;
    void refreshLobbyList();
};