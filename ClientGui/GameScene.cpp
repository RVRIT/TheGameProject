#include "GameScene.h"
#include <iostream>

GameScene::GameScene(sf::Font& f, SceneManager& manager, sf::RenderWindow& window)
    : font(f),
    sceneManager(manager),
    windowRef(window)
{
    // 1. Initialize Game with 1 player
    std::vector<std::string_view> players = { playerName };
    game = std::make_unique<Game>(players);

    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(50.f, 50.f);

    refreshSnapshot();
    std::cout << "GameScene Initialized.\n";
}

void GameScene::refreshSnapshot() {
    currentSnapshot = game->getSnapshot(playerName);

    if (currentSnapshot.isGameOver) {
        if (currentSnapshot.playerWon) statusText.setString("VICTORY! The Game is beaten.");
        else statusText.setString("DEFEAT! No moves possible.");
    }
}

void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if (currentSnapshot.isGameOver) return;

        for (int i = 0; i < 4; ++i) {
            if (getPileBounds(i).contains(mousePos)) {
                if (selectedHandIndex != -1) {
                    bool success = game->attemptPlayCard(selectedHandIndex, i);
                    if (success) {
                        statusText.setString("Card Played!");
                        if (selectedHandIndex >= (int)currentSnapshot.myHand.size() - 1) {
                            selectedHandIndex = -1;
                        }
                        refreshSnapshot();
                    }
                    else {
                        statusText.setString("Invalid Move!");
                    }
                }
                else {
                    std::cout << "Clicked Pile Index: " << i << " (Value: " << currentSnapshot.piles[i].topValue << ")\n";
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

        if (!clickedCard) {
            selectedHandIndex = -1;
        }
    }
}

void GameScene::update(sf::Time)
{
    // Animations logic will go here later
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.draw(statusText);

    for (int i = 0; i < 4; ++i) {
        sf::FloatRect bounds = getPileBounds(i);

        sf::RectangleShape pileShape({ bounds.width, bounds.height });
        pileShape.setPosition(bounds.left, bounds.top);
        pileShape.setOutlineThickness(2);
        pileShape.setOutlineColor(sf::Color::White);

        if (i < 2) pileShape.setFillColor(sf::Color(50, 150, 50));
        else       pileShape.setFillColor(sf::Color(150, 50, 50));

        window.draw(pileShape);

        sf::Text valText;
        valText.setFont(font);
        valText.setString(std::to_string(currentSnapshot.piles[i].topValue));
        valText.setCharacterSize(40);
        valText.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = valText.getLocalBounds();
        valText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        valText.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

        window.draw(valText);

        sf::Text labelText;
        labelText.setFont(font);
        labelText.setString(i < 2 ? "ASC (1->99)" : "DESC (100->2)");
        labelText.setCharacterSize(16);
        labelText.setPosition(bounds.left, bounds.top - 25);
        window.draw(labelText);
    }

    int handSize = (int)currentSnapshot.myHand.size();
    for (int i = 0; i < handSize; ++i) {
        sf::FloatRect bounds = getHandCardBounds(i, handSize);

        sf::RectangleShape cardShape({ bounds.width, bounds.height });
        cardShape.setPosition(bounds.left, bounds.top);
        cardShape.setOutlineThickness(2);

        // Highlight if selected
        if (i == selectedHandIndex) {
            cardShape.setOutlineColor(sf::Color::Yellow);
            cardShape.setFillColor(sf::Color(100, 100, 255));
            cardShape.move(0, -20.f); // Pop up effect
        }
        else {
            cardShape.setOutlineColor(sf::Color::Black);
            cardShape.setFillColor(sf::Color::Blue);
        }

        window.draw(cardShape);

        sf::Text cardVal;
        cardVal.setFont(font);
        cardVal.setString(std::to_string(currentSnapshot.myHand[i]));
        cardVal.setCharacterSize(30);
        cardVal.setFillColor(sf::Color::White);

        float yOffset = (i == selectedHandIndex) ? -20.f : 0.f;

        sf::FloatRect textBounds = cardVal.getLocalBounds();
        cardVal.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        cardVal.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f + yOffset);

        window.draw(cardVal);
    }
}

sf::FloatRect GameScene::getPileBounds(int pileIndex) const {
    float startX = 300.f;
    float startY = 200.f;
    float gapX = 300.f;
    float gapY = 200.f;

    float x = (pileIndex < 2) ? startX : startX + gapX;
    float y = (pileIndex % 2 == 0) ? startY : startY + gapY;

    return sf::FloatRect(x, y, 120.f, 160.f);
}

sf::FloatRect GameScene::getHandCardBounds(int cardIndex, int totalCards) const {
    float cardWidth = 80.f;
    float cardHeight = 120.f;
    float gap = 15.f;

    float totalWidth = (totalCards * cardWidth) + ((totalCards - 1) * gap);
    float startX = (1280.f - totalWidth) / 2.0f;
    float startY = 550.f;

    return sf::FloatRect(startX + cardIndex * (cardWidth + gap), startY, cardWidth, cardHeight);
}