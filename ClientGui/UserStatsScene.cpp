#include "UserStatsScene.h"
#include "MainMenu.h" 
#include "json.hpp"
#include <iostream>
#include <iomanip> 
#include <sstream>

using json = nlohmann::json;

UserStatsScene::UserStatsScene(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& mgr, sf::RenderWindow& win, std::string user)
    : font(fontRef), client(clientRef), sceneManager(mgr), window(win), currentUsername(user),

    backButton("assets/backStats.png", { 540.f, 640.f }, [this]() {
    sceneManager.popScene();
        })
{

    if (bgTexture.loadFromFile("assets/LobbySceneBackground.png")) {
        background.setTexture(bgTexture);
        updateBackgroundScale();
    }

    sf::Vector2u wSize = window.getSize();
    float cardW = 700.f; 
    float cardH = 500.f;
    float cardX = (wSize.x - cardW) / 2.f;
    float cardY = (wSize.y - cardH) / 2.f;

    profileCard.setSize({ cardW, cardH });
    profileCard.setPosition(cardX, cardY);
    profileCard.setFillColor(sf::Color(20, 20, 40, 240));
    profileCard.setOutlineColor(sf::Color::White);
    profileCard.setOutlineThickness(2.f);

    titleText.setFont(font);
    titleText.setString("PLAYER PROFILE");
    titleText.setCharacterSize(40);
    titleText.setStyle(sf::Text::Bold);
    
    sf::FloatRect tr = titleText.getLocalBounds();
    titleText.setPosition(cardX + (cardW - tr.width) / 2.f, cardY + 30.f);

    auto setupText = [&](sf::Text& txt, float yOffset, bool isLabel) {
        txt.setFont(font);
        txt.setCharacterSize(28);

        if (isLabel) {
            txt.setFillColor(sf::Color(200, 200, 200));
            txt.setPosition(cardX + 60.f, cardY + 120.f + yOffset);
        }
      
        };

    float step = 60.f;

    setupText(lblUsername, 0.f, true);  lblUsername.setString("Username:"); 
    setupText(lblRating, step, true);   lblRating.setString("Skill Rating:");
    setupText(lblGames, step * 2, true);  lblGames.setString("Games Played:");
    setupText(lblWins, step * 3, true);   lblWins.setString("Games Won:");
    setupText(lblHours, step * 4, true);  lblHours.setString("Time Played:");

	setupText(valUsername, 0.f, false); valUsername.setPosition(cardX + 330.f, cardY + 120.f );
    setupText(valRating, step, false); valRating.setPosition(cardX + 440.f, cardY + 120.f + step );
    setupText(valGames, step * 2, false); valGames.setPosition(cardX + 430.f, cardY + 120.f + 2*step);
    setupText(valWins, step * 3, false); valWins.setPosition(cardX + 350.f, cardY + 120.f + 3 * step);
    setupText(valHours, step * 4, false); valHours.setPosition(cardX + 410.f, cardY + 120.f + 4 * step);

    fetchAndDisplayStats();
}

void UserStatsScene::fetchAndDisplayStats() {
    std::string jsonStr = client.getUserStats(currentUsername);

    std::string sUser = currentUsername;
    std::string sGames = "0";
    std::string sWins = "0";
    std::string sRating = "0.0";
    std::string sHours = "0.0 hrs";

    if (!jsonStr.empty() && jsonStr != "{}") {
        try {
            auto j = json::parse(jsonStr);
            if (j.contains("username")) sUser = j["username"];
            if (j.contains("games_played")) sGames = std::to_string(j["games_played"].get<int>());
            if (j.contains("games_won")) sWins = std::to_string(j["games_won"].get<int>());

            if (j.contains("rating")) {
                std::ostringstream rateStream;
                rateStream << std::fixed << std::setprecision(1) << j["rating"].get<double>();
                sRating = rateStream.str();
            }

            if (j.contains("hours_played")) {
                std::ostringstream hourStream;
                hourStream << std::fixed << std::setprecision(1) << j["hours_played"].get<double>() << " hrs";
                sHours = hourStream.str();
            }
        }
        catch (...) {}
    }

    valUsername.setString(sUser);
    valGames.setString(sGames);
    valWins.setString(sWins);
    valRating.setString(sRating);
    valHours.setString(sHours);
}

void UserStatsScene::update(sf::Time dt) {}

void UserStatsScene::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(profileCard);
    window.draw(titleText);

    window.draw(lblUsername);
    window.draw(lblRating);
    window.draw(lblGames);
    window.draw(lblWins);
    window.draw(lblHours);

    window.draw(valUsername);
    window.draw(valRating);
    window.draw(valGames);
    window.draw(valWins);
    window.draw(valHours);

    backButton.draw(window);

}

void UserStatsScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    backButton.handleEvent(event, mousePos);
}

void UserStatsScene::updateBackgroundScale() {
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();
    background.setScale(float(win.x) / tex.x, float(win.y) / tex.y);
}