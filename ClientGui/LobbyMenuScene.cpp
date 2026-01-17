#include "LobbyMenuScene.h"
#include "LobbyScene.h"     
#include "GameConstants.h"
#include <iostream>

#include "json.hpp"
using json = nlohmann::json;

LobbyMenuScene::LobbyMenuScene(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, sf::RenderWindow& windowRef, std::string username)
    : font(fontRef), client(clientRef), sceneManager(manager), window(windowRef), currentUsername(username),

    createLobbyBtn("assets/btn_create_lobby.png", { 100.f, 200.f }, [this]() {
    std::cout << "Requesting Create Lobby...\n";
    int newId = this->client.createLobby(this->currentUsername);
    if (newId != -1) {
        sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, newId, this->currentUsername, window));
    }
        }),

    lobbyIdInput(font, { 100.f, 350.f }, { 200.f, 60.f }),

    joinLobbyBtn("assets/btn_join.png", { 100.f, 420.f }, [this]() {
    std::string idStr = lobbyIdInput.getText();
    if (!idStr.empty()) {
        try {
            int id = std::stoi(idStr);
            if (client.joinLobby(id, this->currentUsername)) {
                sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id, this->currentUsername, window));
            }
        }
        catch (...) { std::cout << "Invalid ID format\n"; }
    }
        }),

    backButton("assets/back.png", { 50.f, 800.f }, [this]() {
    sceneManager.popScene(); 
        })
{

    if (bgTexture.loadFromFile("assets/LobbySceneBackground.png")) {
        background.setTexture(bgTexture);
        updateBackgroundScale();

        sf::Vector2u winSize = window.getSize();
        float listX = winSize.x * 0.35f;

        listBackground.setSize({ 600.f, 800.f });
        listBackground.setPosition(listX, 150.f);
        listBackground.setFillColor(sf::Color(0, 0, 0, 150));
        listBackground.setOutlineColor(sf::Color::White);
        listBackground.setOutlineThickness(2.f);

        listTitle.setFont(font);
        listTitle.setString("AVAILABLE LOBBIES");
        listTitle.setCharacterSize(30);
        listTitle.setPosition(listX + 20.f, 100.f);
        listTitle.setFillColor(sf::Color::White);

        refreshLobbyList();
    }



}

void LobbyMenuScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    createLobbyBtn.handleEvent(event, mousePos);
    joinLobbyBtn.handleEvent(event, mousePos);
    backButton.handleEvent(event, mousePos);

    lobbyIdInput.handleEvent(event); 

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        float startX = listBackground.getPosition().x + 10.f;
        float startY = listBackground.getPosition().y + 10.f;
        float gap = 70.f;
        float itemWidth = 580.f;
        float itemHeight = 60.f;

        for (const auto& item : lobbiesData) {
            sf::FloatRect bounds(startX, startY, itemWidth, itemHeight);

            if (bounds.contains(mousePos)) {
                std::string status = item["status"];
                int players = item["playersCount"];
                int maxP = item["maxPlayers"];
                int id = item["id"];

                if (status == "Waiting" && players < maxP) {
                    std::cout << "Joining Lobby " << id << "\n";
                    if (client.joinLobby(id, currentUsername)) {
                        sceneManager.pushScene(std::make_unique<LobbyScene>(font, client, sceneManager, id, currentUsername, window));
                    }
                }
            }
            startY += gap;
        }
    }
}

void LobbyMenuScene::update(sf::Time dt) {
    refreshTimer += dt.asSeconds(); 
    if (refreshTimer > 2.0f) {
        refreshTimer = 0.0f;    
        refreshLobbyList();     
    }
}

void LobbyMenuScene::draw(sf::RenderWindow& window) {
    window.draw(background);

    createLobbyBtn.draw(window);
    lobbyIdInput.draw(window);
    joinLobbyBtn.draw(window);
    backButton.draw(window);

    window.draw(listBackground);
    window.draw(listTitle);

    sf::RectangleShape tempRect;
    sf::Text tempText;
    tempText.setFont(font);

    float startX = listBackground.getPosition().x + 10.f;
    float startY = listBackground.getPosition().y + 10.f;
    float gap = 70.f;

    for (const auto& item : lobbiesData) {
        int id = item["id"];
        std::string host = item["hostName"];
        int players = item["playersCount"];
        int maxP = item["maxPlayers"];
        std::string status = item["status"];

        tempRect.setSize({ 580.f, 60.f });
        tempRect.setPosition(startX, startY);
        tempRect.setOutlineColor(sf::Color::White);
        tempRect.setOutlineThickness(1.f);

        if (status == "InProgress") tempRect.setFillColor(sf::Color(100, 50, 50, 200));
        else if (players >= maxP) tempRect.setFillColor(sf::Color(100, 100, 100, 200));
        else tempRect.setFillColor(sf::Color(50, 150, 50, 150)); 

        window.draw(tempRect);

        std::string info = "ID: " + std::to_string(id) + " | Host: " + host +
            " (" + std::to_string(players) + "/" + std::to_string(maxP) + ")";

        tempText.setString(info);
        tempText.setCharacterSize(20);
        tempText.setFillColor(sf::Color::White);
        tempText.setPosition(startX + 10.f, startY + 18.f);
        window.draw(tempText);

        if (status == "Waiting") tempText.setString("JOIN");
        else tempText.setString(status);

        tempText.setPosition(startX + 560.f, startY + 18.f);
        window.draw(tempText);

        startY += gap;
    }
}

void LobbyMenuScene::refreshLobbyList() {
    std::string jsonResponse = client.getLobbyList();
    if (jsonResponse.empty()) jsonResponse = "[]";

    try {
        lobbiesData = json::parse(jsonResponse);
    }
    catch (...) {
        lobbiesData = json::array(); 
    }
}

void LobbyMenuScene::updateBackgroundScale()
{
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();

    background.setScale(
        float(win.x) / tex.x,
        float(win.y) / tex.y
    );
}