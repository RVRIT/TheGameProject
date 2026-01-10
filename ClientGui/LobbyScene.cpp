#include "LobbyScene.h"
#include "json.hpp"
#include <iostream>
#include "GameScene.h"

using json = nlohmann::json;

LobbyScene::LobbyScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name)
    : font(f), client(c), sceneManager(mgr), lobbyId(id), myName(name),

 
    backButton("assets/back.png", { 50.f, 50.f }, [&]() { mgr.popScene(); }),

    chatInput(font, { 800.f, 600.f }, { 300.f, 40.f }),

    sendButton("assets/btn_send.png", { 1120.f, 600.f }, [this]() {
    std::string msg = chatInput.getText();
    if (!msg.empty()) {
        client.sendLobbyChat(lobbyId, myName, msg);
        chatInput.clear();
    }
        }),

    readyButton("assets/btn_not_ready.png", { 500.f, 500.f }, [this]() {
    if (myPlayerId == -1) return; 

    amIReady = !amIReady;
    client.setPlayerReady(lobbyId, myPlayerId, amIReady);
    std::cout << "Ready clicked. State: " << amIReady << "\n";
        }),

    startGameButton("assets/play.png", { 400.f, 400.f }, [this]() {
    std::cout << "Host clicked Start Game...\n";
    if (myPlayerId != -1) {
        if (client.startGame(lobbyId, myPlayerId)) {
            std::cout << "Start signal sent successfully!\n";
        }
        else {
            std::cout << "Failed to start (Not all ready? Not host?)\n";
        }
    }
        })
{
    titleText.setFont(font);
    titleText.setString("Lobby ID: " + std::to_string(lobbyId));
    titleText.setCharacterSize(40);
    titleText.setPosition(400, 50);

    infoText.setFont(font);
    infoText.setString("Waiting for players...");
    infoText.setCharacterSize(24);
    infoText.setPosition(400, 150);

    chatBackground.setSize({ 400.f, 500.f });
    chatBackground.setFillColor(sf::Color(0, 0, 0, 150));
    chatBackground.setPosition(800.f, 50.f);

    chatDisplay.setFont(font);
    chatDisplay.setCharacterSize(18);
    chatDisplay.setFillColor(sf::Color::White);
    chatDisplay.setPosition(810.f, 60.f);
}

void LobbyScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    backButton.handleEvent(event, mousePos);
    sendButton.handleEvent(event, mousePos);
    readyButton.handleEvent(event, mousePos); 
    chatInput.handleEvent(event);

    if (isHost) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        startGameButton.handleEvent(event, mousePos);
    }
}

void LobbyScene::update(sf::Time dt) {
    static float timer = 0.0f;
    timer += dt.asSeconds();

    if (timer > 0.5f) {
        timer = 0.0f;
        std::string state = client.getLobbyState(lobbyId);
        parseLobbyState(state);
    }
}

void LobbyScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50));

    window.draw(titleText);
    window.draw(infoText);

    backButton.draw(window);
    readyButton.draw(window); 

    window.draw(chatBackground);
    window.draw(chatDisplay);
    chatInput.draw(window);
    sendButton.draw(window);
	readyButton.draw(window);

    if (isHost) {
        startGameButton.draw(window);
    }
}

void LobbyScene::parseLobbyState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;
    try {
        auto j = json::parse(jsonStr);

        if (j.contains("status")) {
            std::string status = j["status"];
            if (status == "InProgress") {
                std::cout << "Server says: Game InProgress! Switching scene...\n";

                sceneManager.changeScene(std::make_unique<GameScene>(font, client, sceneManager, lobbyId, myName));
                return;
            }
        }

        std::string pList = "Players:\n";
        bool firstPlayer = true;

        if (j.contains("players")) {
            for (const auto& p : j["players"]) {
                std::string pName = p["name"];
                int pId = p["id"];
                bool isReady = p["isReady"];

                if (pName == myName) {
                    myPlayerId = pId;
                }

                if (firstPlayer) {
                    if (myPlayerId == pId) isHost = true;
                    else isHost = false;
                    firstPlayer = false;
                }

                std::string statusText = isReady ? " [READY]" : " [...]";
                pList += "- " + pName + statusText + "\n";
            }
        }
        infoText.setString(pList);


    }
    catch (...) {}
}