#include "GameScene.h"
#include <iostream>

GameScene::GameScene(sf::Font& f, SceneManager& manager, sf::RenderWindow& window)
    : font(f),
    sceneManager(manager),
    windowRef(window),
    endTurnButton("assets/BackButton.png", { 1050.f, 600.f }, [this]() {
    if (m_game->attemptEndTurn()) {
        selectedHandIndex = -1;
        logAction("Turn Ended. Hand refilled.");
        refreshSnapshot();
    }
    else {
        logAction("Cannot end turn! Play minimum cards first.");
    }
        })
{
    if (standardCursor.loadFromSystem(sf::Cursor::Arrow)) windowRef.setMouseCursor(standardCursor);
    if (handCursor.loadFromSystem(sf::Cursor::Hand)) {}

    std::vector<std::string_view> m_players = { playerName };
    m_game = std::make_unique<Game>(m_players);

    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50.f, 30.f);

    deckCountText.setFont(font);
    deckCountText.setCharacterSize(20);
    deckCountText.setFillColor(sf::Color::White);
    deckCountText.setPosition(1100.f, 30.f);

    turnInfoText.setFont(font);
    turnInfoText.setCharacterSize(18);
    turnInfoText.setFillColor(sf::Color::Yellow);
    turnInfoText.setPosition(50.f, 70.f);

    logBackground.setSize({ 450.f, 150.f });
    logBackground.setFillColor(sf::Color(0, 0, 0, 150));
    logBackground.setPosition(20.f, 550.f);

    logText.setFont(font);
    logText.setCharacterSize(14);
    logText.setFillColor(sf::Color::White);
    logText.setPosition(30.f, 560.f);

    refreshSnapshot();
    logAction("Game Started.");
}

void GameScene::refreshSnapshot() {
    currentSnapshot = m_game->getSnapshot(playerName);

    if (currentSnapshot.isGameOver) {
        if (currentSnapshot.playerWon) statusText.setString("VICTORY!");
        else statusText.setString("DEFEAT!");
    }
    else {
        statusText.setString("Your Turn: " + playerName);
    }
    updateHUD();
}

void GameScene::logAction(const std::string& msg) {
    gameLogs.push_back(msg);
    if (gameLogs.size() > 6) gameLogs.erase(gameLogs.begin());

    std::string fullString;
    for (const auto& log : gameLogs) fullString += "> " + log + "\n";
    logText.setString(fullString);
}

void GameScene::updateHUD() {
    deckCountText.setString("Deck Size: " + std::to_string(currentSnapshot.deckSize));

    std::string info = "Played: " + std::to_string(currentSnapshot.cardsPlayedThisTurn) +
        " / Required: " + std::to_string(currentSnapshot.minCardsToPlay);
    turnInfoText.setString(info);
}

bool GameScene::isMoveValid(int handIdx, int pileIdx) {
    if (handIdx < 0 || handIdx >= (int)currentSnapshot.myHand.size()) return false;
    if (pileIdx < 0 || pileIdx >= (int)currentSnapshot.piles.size()) return false;

    int cardVal = currentSnapshot.myHand[handIdx];
    const auto& pile = currentSnapshot.piles[pileIdx];
    int topVal = pile.topValue;

    if (pile.type == "ASC") {
        return (cardVal > topVal) || (cardVal == topVal - 10);
    }
    else {
        return (cardVal < topVal) || (cardVal == topVal + 10);
    }
}

void GameScene::updateCursor(sf::RenderWindow& window) {
    if (hoveredHandIndex != -1 || hoveredPileIndex != -1) {
        window.setMouseCursor(handCursor);
    }
    else {
        window.setMouseCursor(standardCursor);
    }
}

void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    endTurnButton.handleEvent(event, mousePos);

    if (event.type == sf::Event::MouseMoved) {
        hoveredHandIndex = -1;
        hoveredPileIndex = -1;

        int handSize = (int)currentSnapshot.myHand.size();
        for (int i = 0; i < handSize; ++i) {
            if (getHandCardBounds(i, handSize).contains(mousePos)) {
                hoveredHandIndex = i;
                break;
            }
        }

        for (int i = 0; i < (int)currentSnapshot.piles.size(); ++i) {
            if (getPileBounds(i).contains(mousePos)) {
                hoveredPileIndex = i;
                break;
            }
        }
        updateCursor(window);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (currentSnapshot.isGameOver) return;

        for (int i = 0; i < (int)currentSnapshot.piles.size(); ++i) {
            if (getPileBounds(i).contains(mousePos)) {
                if (selectedHandIndex != -1) {
                    bool success = m_game->attemptPlayCard(selectedHandIndex, i);

                    if (success) {
                        int val = currentSnapshot.myHand[selectedHandIndex];
                        logAction("Played " + std::to_string(val) + " on Pile " + std::to_string(i + 1));

                        if (selectedHandIndex >= (int)currentSnapshot.myHand.size() - 1) {
                            selectedHandIndex = -1;
                        }
                        refreshSnapshot();
                    }
                    else {
                        logAction("Invalid Move!");
                    }
                }
                return;
            }
        }

        int handSize = (int)currentSnapshot.myHand.size();
        bool clickedCard = false;
        for (int i = 0; i < handSize; ++i) {
            if (getHandCardBounds(i, handSize).contains(mousePos)) {
                if (selectedHandIndex == i) selectedHandIndex = -1;
                else selectedHandIndex = i;
                clickedCard = true;
                break;
            }
        }
        if (!clickedCard) selectedHandIndex = -1;
    }
}

void GameScene::update(sf::Time) {}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(logBackground);
    window.draw(logText);
    window.draw(statusText);
    window.draw(deckCountText);
    window.draw(turnInfoText);
    endTurnButton.draw(window);

    for (size_t i = 0; i < currentSnapshot.piles.size(); ++i) {
        sf::FloatRect bounds = getPileBounds((int)i);
        const auto& pileInfo = currentSnapshot.piles[i];

        sf::RectangleShape pileShape({ bounds.width, bounds.height });
        pileShape.setPosition(bounds.left, bounds.top);
        pileShape.setOutlineThickness(3);

        if ((int)i == hoveredPileIndex) {
            if (selectedHandIndex != -1) {
                if (isMoveValid(selectedHandIndex, (int)i)) pileShape.setOutlineColor(sf::Color::Green);
                else pileShape.setOutlineColor(sf::Color::Red);
            }
            else {
                pileShape.setOutlineColor(sf::Color::Yellow);
            }
        }
        else {
            pileShape.setOutlineColor(sf::Color::White);
        }

        if (pileInfo.type == "ASC") pileShape.setFillColor(sf::Color(50, 150, 50));
        else pileShape.setFillColor(sf::Color(150, 50, 50));

        window.draw(pileShape);

        sf::Text valText;
        valText.setFont(font);
        valText.setString(std::to_string(pileInfo.topValue));
        valText.setCharacterSize(40);
        valText.setFillColor(sf::Color::White);

        sf::FloatRect tb = valText.getLocalBounds();
        valText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
        valText.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        window.draw(valText);

        sf::Text label;
        label.setFont(font);
        label.setString(pileInfo.type);
        label.setCharacterSize(16);
        label.setPosition(bounds.left, bounds.top - 25);
        window.draw(label);
    }

    int handSize = (int)currentSnapshot.myHand.size();
    for (int i = 0; i < handSize; ++i) {
        sf::FloatRect bounds = getHandCardBounds(i, handSize);
        sf::RectangleShape cardShape({ bounds.width, bounds.height });
        cardShape.setPosition(bounds.left, bounds.top);
        cardShape.setOutlineThickness(2);

        float yOffset = 0.f;
        if (i == selectedHandIndex) {
            cardShape.setOutlineColor(sf::Color::Yellow);
            cardShape.setFillColor(sf::Color(100, 100, 255));
            yOffset = -20.f;
        }
        else if (i == hoveredHandIndex) {
            cardShape.setOutlineColor(sf::Color::White);
            cardShape.setFillColor(sf::Color(80, 80, 220));
            yOffset = -10.f;
        }
        else {
            cardShape.setOutlineColor(sf::Color::Black);
            cardShape.setFillColor(sf::Color::Blue);
        }

        cardShape.move(0, yOffset);
        window.draw(cardShape);

        sf::Text cardVal;
        cardVal.setFont(font);
        cardVal.setString(std::to_string(currentSnapshot.myHand[i]));
        cardVal.setCharacterSize(30);
        cardVal.setFillColor(sf::Color::White);

        sf::FloatRect tb = cardVal.getLocalBounds();
        cardVal.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
        cardVal.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f + yOffset);
        window.draw(cardVal);
    }
}

sf::FloatRect GameScene::getPileBounds(int pileIndex) const {
    float startX = 300.f;
    float startY = 200.f;
    float gapX = 200.f;
    return sf::FloatRect(startX + pileIndex * gapX, startY, 120.f, 160.f);
}

sf::FloatRect GameScene::getHandCardBounds(int cardIndex, int totalCards) const {
    float cardWidth = 80.f;
    float cardHeight = 120.f;
    float gap = 15.f;
    float totalWidth = (totalCards * cardWidth) + ((totalCards - 1) * gap);
    float startX = (1280.f - totalWidth) / 2.0f;
    return sf::FloatRect(startX + cardIndex * (cardWidth + gap), 550.f, cardWidth, cardHeight);
}