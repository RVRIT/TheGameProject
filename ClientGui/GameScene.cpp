#include "GameScene.h"
#include "MainMenu.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

GameScene::GameScene(sf::Font& f, NetworkClient& c, SceneManager& mgr, int id, std::string name, sf::RenderWindow& win)
    : font(f), client(c), sceneManager(mgr), lobbyId(id), myName(name), window(win),

    endTurnButton("assets/nextTurn.png", { 1600.f, 700.f }, [this]() {
    if (!isGameOver) {
        this->client.endTurn(lobbyId, myName);
    }
        }),

    backButton("assets/exit.png", { 860.f, 600.f }, [this]() {
    sceneManager.changeScene(std::make_unique<MainMenu>(font, client, sceneManager, window, myName));
        }),

    btnExit("assets/exit.png", { 860.f, 550.f }, [this]() {
    std::cout << "Exiting game...\n";
    client.leaveLobby(lobbyId, myName);

    window.setSize(sf::Vector2u(1280, 720));
    sf::View view(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
    window.setView(view);
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        (desktop.width - 1280) / 2,
        (desktop.height - 720) / 2));

    sceneManager.changeScene(std::make_unique<MainMenu>(font, client, sceneManager, window, myName));
        })

{

    if (bgTexture.loadFromFile("assets/gameBG.png")) {
        background.setTexture(bgTexture);
		updateBackgroundScale();
        }
 
    if (sf::Shader::isAvailable()) {
        if (bgShader.loadFromFile("assets/background.frag", sf::Shader::Fragment)) {
            shaderLoaded = true;

            bgShader.setUniform("iResolution", sf::Vector2f(1920.f, 1080.f));
        }
        else {
            std::cout << "[WARNING] Shader failed to load. Using background.\n";
        }
    }

    shaderRect.setSize(sf::Vector2f(1920.f, 1080.f));
    shaderRect.setPosition(0, 0);
    
    if (!cardTexture.loadFromFile("assets/card_base.png")) { }

    sf::View view(sf::FloatRect(0.f, 0.f, 1920.f, 1080.f));
    window.setView(view);

    statusText.setPosition(50, 50);

    cardsPlayedText.setFont(font);
    cardsPlayedText.setCharacterSize(24);
    cardsPlayedText.setFillColor(sf::Color::Yellow);
    cardsPlayedText.setString("Played: - / -");

    deckInfoText.setFont(font);
    deckInfoText.setCharacterSize(24);
    deckInfoText.setFillColor(sf::Color::White);
    deckInfoText.setPosition(50.f, 150.f); 
    deckInfoText.setString("Deck: --");

    isMenuOpen = false;
    menuOverlay.setSize({ 300.f, 380.f });
    menuOverlay.setFillColor(sf::Color(0, 0, 0, 150)); 
    menuOverlay.setPosition(850.f, 500.f);

}

void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (isGameOver) {
        backButton.handleEvent(event, mousePos);
        return;
    }

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
        isMenuOpen = !isMenuOpen; 
    }

    if (isMenuOpen) {

        btnExit.handleEvent(event, mousePos);
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

    bgShader.setUniform("iTime", shaderTimer.getElapsedTime().asSeconds());
}

void GameScene::parseGameState(const std::string& jsonStr) {
    if (jsonStr.empty()) return;

    try {
        auto j = json::parse(jsonStr);

        if (j.contains("cardsPlayedThisTurn") && j.contains("minCardsToPlay")) {
            int played = j["cardsPlayedThisTurn"];
            int minReq = j["minCardsToPlay"];

            cardsPlayedText.setString("Played: " + std::to_string(played) + " / " + std::to_string(minReq));

            sf::FloatRect tr = cardsPlayedText.getLocalBounds();
            cardsPlayedText.setPosition(50.f, 50.f);

            if (played >= minReq) cardsPlayedText.setFillColor(sf::Color::Green);
            else cardsPlayedText.setFillColor(sf::Color::Yellow);
        }

        if (j.contains("isGameOver") && j["isGameOver"].get<bool>()) {
            this->isGameOver = true;
            bool won = j["playerWon"];
            statusText.setString(won ? "VICTORY!" : "GAME OVER");
            statusText.setFillColor(won ? sf::Color::Green : sf::Color::Red);
            statusText.setCharacterSize(60);

            sf::FloatRect sr = statusText.getLocalBounds();
            statusText.setPosition((1920.f - sr.width) / 2.f, 200.f);
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

            float cardWidth = 100.f; 
            float gap = 50.f;        
            float totalWidth = (4 * cardWidth) + (3 * gap); 
            float startX = (1920.f - totalWidth) / 2.f;     
            float pileY = 350.f;                            

            for (const auto& p : j["piles"]) {
                VisualPile vp;
                std::string typeStr = p["type"];
                vp.type = (typeStr == "ASC") ? 0 : 1;
                int val = p["topValue"];

                vp.card = std::make_unique<Card>(cardTexture, font, val);

                float currentX = startX + (pIndex * (cardWidth + gap));
                vp.card->setPosition(currentX, pileY);

                piles.push_back(std::move(vp));
                pIndex++;
            }
        }

        myHand.clear();
        int cIndex = 0;
        if (j.contains("myHand")) {
 
            float startX = 400.f;
            float gap = 140.f;

            for (const auto& val : j["myHand"]) {
                auto card = std::make_unique<Card>(cardTexture, font, val);
                card->setPosition(startX + (cIndex * gap), 850.f);

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

    window.clear(sf::Color::Black);


    if (shaderLoaded==true) {
        window.draw(shaderRect, &bgShader);
    }
    else {
        window.draw(background);
    }


    if (isMenuOpen) {
        window.draw(menuOverlay);   
        btnExit.draw(window);       

    }

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

	window.draw(cardsPlayedText);
}

void GameScene::updateBackgroundScale()
{
    sf::Vector2u win = window.getSize();
    sf::Vector2u tex = bgTexture.getSize();

    background.setScale(
        float(win.x) / tex.x,
        float(win.y) / tex.y
    );

}