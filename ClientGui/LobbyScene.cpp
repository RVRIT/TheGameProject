#include "LobbyScene.h"
#include "json.hpp"
#include <iostream>

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
	readyButton.handleEvent(event, mousePos);
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
}

void LobbyScene::parseLobbyState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr);

        std::string pList = "Players:\n";
        if (j.contains("players")) {
            for (const auto& p : j["players"]) {
                std::string pName = p["name"];
                int pId = p["id"];
                bool isReady = p["isReady"];

                if (pName == myName) {
                    myPlayerId = pId;
                }

                std::string status = isReady ? " [READY]" : " [...]";
                pList += "- " + pName + status + "\n";
            }
        }
        infoText.setString(pList);

        std::string cHist = "";
        if (j.contains("chat")) {
            for (const auto& msg : j["chat"]) {
                cHist += std::string(msg["sender"]) + ": " + std::string(msg["content"]) + "\n";
            }
        }
        chatDisplay.setString(cHist);

    }
    catch (...) {
       
    }
}