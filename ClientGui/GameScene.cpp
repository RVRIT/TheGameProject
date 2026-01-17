#include "GameScene.h"
#include "MainMenu.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

GameScene::GameScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name, sf::RenderWindow& win)
    : font(f), client(c), sceneManager(mgr), lobbyId(id), myName(name), window(win),

    endTurnButton("assets/exit.png", { 1000.f, 600.f }, [this]() {
    if (!isGameOver) {
        std::cout << "Requesting End Turn...\n";
        this->client.endTurn(lobbyId, myName);
    }
        }),

    backButton("assets/exit.png", { 500.f, 400.f }, [this]() {
    std::cout << "Returning to Main Menu...\n";
    sceneManager.changeScene(std::make_unique<MainMenu>(font, client, sceneManager, window, myName));
        })
{
    if (bgTexture.loadFromFile("assets/gameBG.png")) {
        background.setTexture(bgTexture);

    }

    if (!cardTexture.loadFromFile("assets/card_base.png")) {
        std::cout << "ERROR: Could not load assets/card_base.png\n";
    }

    statusText.setFont(font);
    statusText.setCharacterSize(12);
    statusText.setPosition(20.f, 20.f);
    statusText.setFillColor(sf::Color::White);

    deckInfoText.setFont(font);
    deckInfoText.setCharacterSize(20);
    deckInfoText.setPosition(20.f, 60.f);
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

        for (size_t i = 0; i < myHand.size(); ++i) {
            if (myHand[i]->getBounds().contains(mousePos)) {

                if (selectedHandIndex != -1 && selectedHandIndex < myHand.size()) {
                    myHand[selectedHandIndex]->deselect();
                }

                selectedHandIndex = static_cast<int>(i);

                myHand[i]->select();
                return;
            }
        }

        if (selectedHandIndex != -1) {
            for (size_t i = 0; i < piles.size(); ++i) {
                if (piles[i].card->getBounds().contains(mousePos)) {

                    std::cout << "Trying to play card index " << selectedHandIndex << " on pile " << i << "\n";

                    if (client.playCard(lobbyId, myName, selectedHandIndex, static_cast<int>(i))) {
                        if (selectedHandIndex < myHand.size()) myHand[selectedHandIndex]->deselect();
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

void GameScene::parseGameState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr);

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

        piles.clear();
        int pIndex = 0;
        if (j.contains("piles")) {
            for (const auto& p : j["piles"]) {
                VisualPile vp;
                std::string typeStr = p["type"];
                vp.type = (typeStr == "ASC") ? 0 : 1;
                int val = p["topValue"];

                vp.card = std::make_unique<Card>(cardTexture, font, val);

                vp.card->setPosition(300.f + (pIndex * 150), 300.f); 

                piles.push_back(std::move(vp));
                pIndex++;
            }
        }

        myHand.clear();
        int cIndex = 0;
        if (j.contains("myHand")) {
            for (const auto& val : j["myHand"]) {
                auto card = std::make_unique<Card>(cardTexture, font, val);

                card->setPosition(150.f + (cIndex * 90), 600.f);

                if (cIndex == selectedHandIndex) {
                    card->select();
                }

                myHand.push_back(std::move(card));
                cIndex++;
            }
        }

        if (selectedHandIndex >= static_cast<int>(myHand.size())) {
            selectedHandIndex = -1;
        }

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

        if (j.contains("deckSize")) {
            deckInfoText.setString("Deck: " + std::to_string(j["deckSize"].get<int>()));
        }

    }
    catch (const std::exception& e) {
        std::cout << "JSON Parse Error: " << e.what() << "\n";
    }
}

void GameScene::draw(sf::RenderWindow& window) {

    window.draw(background);
    if (isGameOver) {
        window.draw(statusText);
        backButton.draw(window);
        return;
    }

    window.draw(statusText);
    window.draw(deckInfoText);
    endTurnButton.draw(window);

    float oppX = 400.f;
    for (const auto& opp : opponents) {
        sf::Text t;
        t.setFont(font);
        std::string marker = opp.isTurn ? " [TURN]" : "";
        t.setString(opp.name + "\nCards: " + std::to_string(opp.cardCount) + marker);
        t.setCharacterSize(18);
        t.setPosition(oppX, 50.f);
        t.setFillColor(opp.isTurn ? sf::Color::Yellow : sf::Color::White);
        window.draw(t);
        oppX += 200.f;
    }

    for (const auto& p : piles) {
        sf::Text label;
        label.setFont(font);
        label.setString(p.type == 0 ? "UP" : "DOWN");
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::White);
        label.setStyle(sf::Text::Bold);

        sf::FloatRect cardBounds = p.card->getBounds();
        label.setPosition(cardBounds.left, cardBounds.top - 25.f);

        window.draw(label);

        p.card->draw(window);
    }

    for (const auto& card : myHand) {
        card->draw(window);
    }
}