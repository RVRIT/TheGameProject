#include "GameScene.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

GameScene::GameScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name)
    : font(f), client(c), sceneManager(mgr), lobbyId(id), myName(name),
    endTurnButton("assets/exit.png", { 1000.f, 600.f }, [this]() {
    std::cout << "Requesting End Turn...\n";
    this->client.endTurn(lobbyId, myName);
        })
{
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setPosition(50, 50);
    statusText.setFillColor(sf::Color::White);

    deckInfoText.setFont(font);
    deckInfoText.setCharacterSize(20);
    deckInfoText.setPosition(50, 100);
    deckInfoText.setFillColor(sf::Color::White);
}

void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    endTurnButton.handleEvent(event, mousePos);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        for (size_t i = 0; i < myHand.size(); ++i) {
            if (myHand[i].bounds.contains(mousePos)) {
                selectedHandIndex = static_cast<int>(i);
                std::cout << "Selected card: " << myHand[i].value << "\n";
                return; // Am găsit cartea, ieșim
            }
        }

        if (selectedHandIndex != -1) {
            for (size_t i = 0; i < piles.size(); ++i) {
                if (piles[i].bounds.contains(mousePos)) {
                    std::cout << "Playing card on Pile " << i << "...\n";

                    if (client.playCard(lobbyId, myName, selectedHandIndex, static_cast<int>(i))) {
                        std::cout << "Move accepted!\n";
                        selectedHandIndex = -1; 
                        update(sf::seconds(1.0f)); 
                    }
                    else {
                        std::cout << "Move invalid!\n";
                    }
                }
            }
        }
    }
}

void GameScene::update(sf::Time dt) {
    static float timer = 0.0f;
    timer += dt.asSeconds();

    if (timer > 0.5f) {
        timer = 0.0f;
        std::string jsonState = client.getGameState(lobbyId, myName);
        parseGameState(jsonState);
    }
}

void GameScene::parseGameState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr);

        piles.clear();
        int pIndex = 0;
        float startX = 300.f;
        if (j.contains("piles")) {
            for (const auto& p : j["piles"]) {
                VisualPile vp;
                vp.type = p["type"];
                vp.topValue = p["topValue"];
                vp.bounds = sf::FloatRect(startX + (pIndex * 150), 300.f, 100.f, 140.f);
                piles.push_back(vp);
                pIndex++;
            }
        }

        myHand.clear();
        int cIndex = 0;
        float handX = 150.f;
        if (j.contains("myHand")) {
            for (const auto& val : j["myHand"]) {
                VisualCard vc;
                vc.value = val;
                vc.isSelected = (cIndex == selectedHandIndex);
                vc.bounds = sf::FloatRect(handX + (cIndex * 90), 550.f, 70.f, 100.f);
                myHand.push_back(vc);
                cIndex++;
            }
        }
        if (selectedHandIndex >= static_cast<int>(myHand.size())) selectedHandIndex = -1;

        opponents.clear();
        if (j.contains("opponents")) {
            for (const auto& opp : j["opponents"]) {
                VisualOpponent vo;
                vo.name = opp["name"];
                vo.cardCount = opp["cardCount"];
                vo.isTurn = opp["isTurn"];
                opponents.push_back(vo);
            }
        }

        if (j.contains("deckSize")) {
            deckInfoText.setString("Deck: " + std::to_string(j["deckSize"].get<int>()));
        }

        if (j.contains("isGameOver") && j["isGameOver"].get<bool>()) {
            bool won = j["playerWon"];
            statusText.setString(won ? "VICTORY!" : "GAME OVER - DEFEAT");
            statusText.setFillColor(won ? sf::Color::Green : sf::Color::Red);
        }
        else {
            statusText.setString("Game In Progress");
        }

    }
    catch (...) {
        std::cout << "Error parsing game state JSON\n";
    }
}

void GameScene::drawCard(sf::RenderWindow& window, int value, sf::Vector2f pos, bool isSelected, bool isPile, int pileType) {
    sf::RectangleShape rect(sf::Vector2f(isPile ? 100.f : 70.f, isPile ? 140.f : 100.f));
    rect.setPosition(pos);

    if (isPile) {
        rect.setFillColor(pileType == 0 ? sf::Color(240, 240, 240) : sf::Color(150, 150, 150));
        rect.setOutlineColor(sf::Color::Yellow);
    }
    else {
        rect.setFillColor(isSelected ? sf::Color::Green : sf::Color::White);
    }

    rect.setOutlineThickness(2.f);
    window.draw(rect);

    sf::Text t;
    t.setFont(font);
    t.setString(std::to_string(value));
    t.setCharacterSize(isPile ? 30 : 24);
    t.setFillColor(sf::Color::Black);

    sf::FloatRect tr = t.getLocalBounds();
    t.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
    t.setPosition(pos.x + rect.getSize().x / 2.0f, pos.y + rect.getSize().y / 2.0f);
    window.draw(t);

    if (isPile) {
        sf::Text label;
        label.setFont(font);
        label.setString(pileType == 0 ? "UP (1->99)" : "DOWN (100->2)");
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::Red);
        label.setPosition(pos.x, pos.y - 15.f);
        window.draw(label);
    }
}

void GameScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(34, 139, 34)); 

    window.draw(statusText);
    window.draw(deckInfoText);
    endTurnButton.draw(window);

    float oppX = 300.f;
    for (const auto& opp : opponents) {
        sf::Text t;
        t.setFont(font);
        std::string marker = opp.isTurn ? " [TURN]" : "";
        t.setString(opp.name + "\nCards: " + std::to_string(opp.cardCount) + marker);
        t.setCharacterSize(18);
        t.setPosition(oppX, 30.f);
        t.setFillColor(opp.isTurn ? sf::Color::Yellow : sf::Color::White);
        window.draw(t);
        oppX += 250.f;
    }

    for (const auto& p : piles) {
        drawCard(window, p.topValue, { p.bounds.left, p.bounds.top }, false, true, p.type);
    }

    for (const auto& c : myHand) {
        drawCard(window, c.value, { c.bounds.left, c.bounds.top }, c.isSelected, false);
    }
}