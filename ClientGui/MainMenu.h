#pragma once
#include <SFML/Graphics.hpp>
#include "button.h"
#include "Scene.h"  
#include "NetworkClient.h"
#include "TextBox.h"
#include <string>


class SceneManager; 

class MainMenu : public Scene 
{
public:
    NetworkClient& client;
    MainMenu(sf::Font& font, NetworkClient& client, SceneManager& manager, sf::RenderWindow& window, std::string username);
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderWindow& window) override; 

    void updateBackgroundScale();

private:
    sf::Font& font;
    SceneManager& sceneManager; 
    sf::RenderWindow& window;
    Button playButton;
    Button exitButton;
    Button settingsButton;
    sf::Texture bgTexture;
    sf::Sprite background;

    Button createLobbyButton;   

    TextBox lobbyIdInput;
    Button joinLobbyButton;
    std::string currentUsername;
};