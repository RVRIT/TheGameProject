#include "LobbyScene.h"
#include "json.hpp"
#include <iostream>
#include "GameScene.h"

using json = nlohmann::json;

LobbyScene::LobbyScene(sf::Font& fontRef, NetworkClient& clientRef, SceneManager& manager, int id, std::string name, sf::RenderWindow& win)
    : font(fontRef), client(clientRef), sceneManager(manager), lobbyId(id), myName(name), window(win),

 
    backButton("assets/back.png", { 50.f, 50.f }, [&]() { manager.popScene(); }),

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

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        std::string msg = chatInput.getText();
        if (!msg.empty()) {
            client.sendLobbyChat(lobbyId, myName, msg);
            chatInput.clear();
        }
    }

    if (isHost) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        startGameButton.handleEvent(event, mousePos);
    }

    if (isHost && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        float startY = 150.f;
        float gapY = 40.f;

        for (size_t i = 0; i < playerList.size(); ++i) {
            if (playerList[i].first == myName) continue;

            float currentY = startY + (i * gapY);

            sf::FloatRect btnRect(350.f, currentY, 25.f, 25.f);

            if (btnRect.contains(mousePos)) {
                std::cout << "Kicking ID: " << playerList[i].first << "\n";

                client.kickPlayer(lobbyId, myName, playerList[i].first);
            }
        }
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

    float startY = 150.f;
    float gapY = 40.f;    

    for (size_t i = 0; i < playerList.size(); ++i) {
        float currentY = startY + (i * gapY);

        sf::Text t;
        t.setFont(font);
        t.setString(playerList[i].second);
        t.setCharacterSize(24);
        t.setPosition(50.f, currentY);
        t.setFillColor(sf::Color::White);
        window.draw(t);

        if (isHost && playerList[i].first != myName) {

            sf::RectangleShape kickBtn(sf::Vector2f(25.f, 25.f));
            kickBtn.setPosition(350.f, currentY);
            kickBtn.setFillColor(sf::Color::Red);
            kickBtn.setOutlineColor(sf::Color::White);
            kickBtn.setOutlineThickness(1.f);
            window.draw(kickBtn);

            sf::Text xText("X", font, 18);
            xText.setPosition(356.f, currentY - 2.f);
            xText.setFillColor(sf::Color::White);
            window.draw(xText);
        }
    }


}

void LobbyScene::parseLobbyState(const std::string& jsonStr) {
    std::cout << "[DEBUG JSON]: " << jsonStr << std::endl;
    if (jsonStr.empty()) return;
    try {
        auto j = json::parse(jsonStr);

        if (j.contains("status")) {
            std::string status = j["status"];
            if (status == "InProgress") {
                std::cout << "Server says: Game InProgress! Switching scene...\n";
                window.create(sf::VideoMode(1920, 1080), "The Game", sf::Style::Fullscreen);
                window.setFramerateLimit(60);
                window.setMouseCursorVisible(true);

                sceneManager.changeScene(std::make_unique<GameScene>(font, client, sceneManager, lobbyId, myName, window));
                return;
            }
        }

        if (j.contains("chat") && !j["chat"].is_null()) {
            std::string allMessages = "";

            for (const auto& msgObj : j["chat"]) {
                if (msgObj.contains("sender") && msgObj.contains("content")) {
                    std::string sender = msgObj["sender"];
                    std::string content = msgObj["content"];

                    allMessages += sender + ": " + content + "\n";
                }
            }
            chatDisplay.setString(allMessages);
        }

        playerList.clear();
        bool firstPlayer = true;

        if (j.contains("players")) {
            for (const auto& p : j["players"]) {
                std::string pName = p["name"]; 
                int pId = p["id"];
                bool isReady = p["isReady"];

                if (pName == myName) myPlayerId = pId;

                if (firstPlayer) {
                    if (pName == myName) isHost = true;
                    else isHost = false;
                    firstPlayer = false; 
                }

                std::string displayName = pName;

                if (isReady) displayName += " [READY]";
                else displayName += " [...]";

                if (j["players"][0]["name"] == pName) displayName += " (Host)";

                playerList.push_back({ pName, displayName });
            }
        }
    }
    catch (...) {}
}