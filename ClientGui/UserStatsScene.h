#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NetworkClient.h"
#include "Button.h"
#include <SFML/Graphics.hpp>

class UserStatsScene : public Scene {
public:
    UserStatsScene(sf::Font& font, NetworkClient& client, SceneManager& manager, sf::RenderWindow& window, std::string username);

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

    // --- UI Elements ---
    sf::Sprite background;
    sf::Texture bgTexture;

    // Cardul Profilului
    sf::RectangleShape profileCard;
    sf::Text titleText;

    // Labels (Stânga)
    sf::Text lblUsername;
    sf::Text lblRating;
    sf::Text lblGames;
    sf::Text lblWins;
    sf::Text lblHours;

    // Values (Dreapta - Datele dinamice)
    sf::Text valUsername;
    sf::Text valRating;
    sf::Text valGames;
    sf::Text valWins;
    sf::Text valHours;

    Button backButton;

    void fetchAndDisplayStats();
};