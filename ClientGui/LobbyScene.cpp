#include "LobbyScene.h"
#include <string>
using json = nlohmann::json;

LobbyScene::LobbyScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id)
    : font(f), client(c), sceneManager(mgr), lobbyId(id),
    backButton("assets/back.png", { 50.f, 50.f }, [&]() { mgr.popScene(); }) ,
    chatInput(font, { 800.f, 600.f }, { 300.f, 40.f }),

    // Initialize Send Button (Next to Input)
    sendButton("assets/btn_send.png", { 1120.f, 600.f }, [this]() {
    std::string msg = chatInput.getText();
    if (!msg.empty()) {
        // Send to Server (Hardcoded name "Player" for now)
        client.sendLobbyChat(lobbyId, "Me", msg);
        chatInput.clear(); // You might need to add a clear() method to TextBox
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
    chatBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    chatBackground.setPosition(800.f, 50.f);

    chatDisplay.setFont(font);
    chatDisplay.setCharacterSize(18);
    chatDisplay.setFillColor(sf::Color::White);
    chatDisplay.setPosition(810.f, 60.f);
}

void LobbyScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    backButton.handleEvent(event, mousePos);
	chatInput.handleEvent(event);
	sendButton.handleEvent(event, mousePos);
    
}

void LobbyScene::update(sf::Time dt) {
    static float timer = 0.0f;
    timer += dt.asSeconds();

    // Poll every 0.5 seconds for snappier chat
    if (timer > 0.5f) {
        timer = 0.0f;
        std::string state = client.getLobbyState(lobbyId);
        parseLobbyState(state);
    }
}

void LobbyScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50)); // Dark Grey Background
    window.draw(titleText);
    window.draw(infoText);
    backButton.draw(window);
	window.draw(chatBackground);
    window.draw(chatDisplay);
    chatInput.draw(window);
	sendButton.draw(window);

}

void LobbyScene::parseLobbyState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr);

        // 1. Parse Players (Update Info Text)
        std::string pList = "Players in Lobby:\n";
        if (j.contains("players")) {
            for (const auto& p : j["players"]) {
                pList += "- " + std::string(p["name"]) + "\n";
            }
        }
        infoText.setString(pList);

        // 2. Parse Chat (Update Chat Display)
        std::string cHist = "";
        if (j.contains("chat")) {
            for (const auto& msg : j["chat"]) {
                cHist += std::string(msg["sender"]) + ": " + std::string(msg["content"]) + "\n";
            }
        }
        chatDisplay.setString(cHist);

    }
    catch (...) {
        // Ignore parsing errors for now
    }
}