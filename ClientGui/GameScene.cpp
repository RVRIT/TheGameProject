#include "GameScene.h"
#include "MainMenu.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

// 1. Constructorul reparat (primește window)
GameScene::GameScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name, sf::RenderWindow& win)
    : font(f), client(c), sceneManager(mgr), lobbyId(id), myName(name), window(win), // Init window

    endTurnButton("assets/exit.png", { 1000.f, 600.f }, [this]() {
    if (!isGameOver) {
        std::cout << "Requesting End Turn...\n";
        this->client.endTurn(lobbyId, myName);
    }
        }),

    // 2. Butonul Back reparat (folosește 'window' local, nu getWindow)
    backButton("assets/exit.png", { 500.f, 400.f }, [this]() {
    std::cout << "Returning to Main Menu...\n";
    sceneManager.changeScene(std::make_unique<MainMenu>(font, client, sceneManager, window, myName));
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

    if (isGameOver) {
        backButton.handleEvent(event, mousePos);
        return;
    }

    endTurnButton.handleEvent(event, mousePos);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Click pe cartile din mana
        for (size_t i = 0; i < myHand.size(); ++i) {
            if (myHand[i].bounds.contains(mousePos)) {
                selectedHandIndex = static_cast<int>(i);
                return;
            }
        }
        // Click pe teancuri
        if (selectedHandIndex != -1) {
            for (size_t i = 0; i < piles.size(); ++i) {
                if (piles[i].bounds.contains(mousePos)) {
                    if (client.playCard(lobbyId, myName, selectedHandIndex, static_cast<int>(i))) {
                        selectedHandIndex = -1;
                        update(sf::seconds(1.0f));
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

// 3. Funcția parseGameState reparată (fără erori de sintaxă la 'j')
void GameScene::parseGameState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr); // Aici se definește 'j'

        // 1. Verificăm Game Over
        if (j.contains("isGameOver") && j["isGameOver"].get<bool>()) {
            this->isGameOver = true;
            bool won = j["playerWon"];
            statusText.setString(won ? "VICTORY!" : "GAME OVER");
            statusText.setFillColor(won ? sf::Color::Green : sf::Color::Red);
            statusText.setCharacterSize(60);
            statusText.setPosition(400.f, 200.f);
        }
        else {
            this->isGameOver = false;
            statusText.setCharacterSize(24);
            statusText.setPosition(50, 50);
            statusText.setString("Game In Progress");
            statusText.setFillColor(sf::Color::White);
        }

        // 2. Teancuri
        piles.clear();
        int pIndex = 0;
        if (j.contains("piles")) {
            for (const auto& p : j["piles"]) {
                VisualPile vp;
                std::string typeStr = p["type"];
                vp.type = (typeStr == "ASC") ? 0 : 1;
                vp.topValue = p["topValue"];
                vp.bounds = sf::FloatRect(300.f + (pIndex * 120), 300.f, 100.f, 140.f);
                piles.push_back(vp);
                pIndex++;
            }
        }

        // 3. Mână
        myHand.clear();
        int cIndex = 0;
        if (j.contains("myHand")) {
            for (const auto& val : j["myHand"]) {
                VisualCard vc;
                vc.value = val;
                vc.isSelected = (cIndex == selectedHandIndex);
                vc.bounds = sf::FloatRect(150.f + (cIndex * 80), 550.f, 70.f, 100.f);
                myHand.push_back(vc);
                cIndex++;
            }
        }
        if (selectedHandIndex >= static_cast<int>(myHand.size())) selectedHandIndex = -1;

        // 4. Adversari
        opponents.clear();
        if (j.contains("opponents")) {
            for (const auto& opp : j["opponents"]) {
                VisualOpponent vo;
                vo.name = opp["name"];
                vo.cardCount = opp["cardCount"];
                vo.isTurn = opp.contains("isTurn") ? opp["isTurn"].get<bool>() : false;
                opponents.push_back(vo);
            }
        }

        // 5. Deck info
        if (j.contains("deckSize")) {
            deckInfoText.setString("Deck: " + std::to_string(j["deckSize"].get<int>()));
        }

    }
    catch (...) {
        std::cout << "Error parsing JSON in GameScene\n";
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
        label.setString(pileType == 0 ? "UP" : "DOWN");
        label.setCharacterSize(14);
        label.setFillColor(sf::Color::Red);
        label.setPosition(pos.x, pos.y - 20.f);
        window.draw(label);
    }
}

void GameScene::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(34, 139, 34));

    if (isGameOver) {
        window.draw(statusText);
        backButton.draw(window);
        return;
    }

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
        oppX += 200.f;
    }

    for (const auto& p : piles) {
        drawCard(window, p.topValue, { p.bounds.left, p.bounds.top }, false, true, p.type);
    }

    for (const auto& c : myHand) {
        drawCard(window, c.value, { c.bounds.left, c.bounds.top }, c.isSelected, false);
    }
}