//#include "GameScene.h"
//#include <iostream>
//
//GameScene::GameScene(sf::Font& font)
//    : m_font(font)
//{
//    m_cardTexture.loadFromFile("assets/cardBase.png");
//    m_cardSprite.setTexture(m_cardTexture);
//
//    m_snapshot = MockGameManager::get().game().getSnapshot("Player1");
//}
//
//void GameScene::update(sf::Time dt)
//{
//    m_snapshot = MockGameManager::get().game().getSnapshot("Player1");
//}
//
//void GameScene::handleEvent(const sf::Event& event, sf::RenderWindow& window)
//{
//    Game& game = MockGameManager::get().game();
//
//    if (event.type == sf::Event::MouseButtonPressed)
//    {
//        sf::Vector2f mousePos =
//            window.mapPixelToCoords(sf::Mouse::getPosition(window));
//
//        // Select card
//        for (int i = 0; i < m_snapshot.myHand.size(); i++)
//        {
//            if (getCardBounds(i).contains(mousePos))
//            {
//                m_selectedCard = i;
//                return;
//            }
//        }
//
//        // Drop card on pile
//        if (m_selectedCard != -1)
//        {
//            for (int p = 0; p < 4; p++)
//            {
//                if (getPileBounds(p).contains(mousePos))
//                {
//                    if (game.attemptPlayCard(m_selectedCard, p))
//                        m_selectedCard = -1;
//
//                    return;
//                }
//            }
//        }
//    }
//}
//
//void GameScene::draw(sf::RenderWindow& window)
//{
//    // Draw Piles
//    float px = 200;
//    float py = 100;
//
//    for (int i = 0; i < 4; i++)
//    {
//        m_cardSprite.setPosition(px, py);
//        window.draw(m_cardSprite);
//
//        sf::Text t(std::to_string(m_snapshot.piles[i].topValue), m_font, 26);
//        t.setFillColor(sf::Color::Black);
//        t.setPosition(px + 40, py + 25);
//        window.draw(t);
//
//        px += 200;
//    }
//
//    // Draw Hand
//    float hx = 100;
//    float hy = 600;
//
//    for (int i = 0; i < m_snapshot.myHand.size(); i++)
//    {
//        m_cardSprite.setPosition(hx, hy);
//
//        if (i == m_selectedCard)
//            m_cardSprite.setColor(sf::Color(200, 255, 200));
//        else
//            m_cardSprite.setColor(sf::Color::White);
//
//        window.draw(m_cardSprite);
//
//        sf::Text t(std::to_string(m_snapshot.myHand[i]), m_font, 26);
//        t.setFillColor(sf::Color::Black);
//        t.setPosition(hx + 40, hy + 25);
//        window.draw(t);
//
//        hx += 90;
//    }
//}
//
//sf::FloatRect GameScene::getCardBounds(int idx) const
//{
//    float x = 100 + idx * 90;
//    float y = 600;
//
//    return { x, y,
//             (float)m_cardTexture.getSize().x,
//             (float)m_cardTexture.getSize().y };
//}
//
//sf::FloatRect GameScene::getPileBounds(int idx) const
//{
//    float x = 200 + idx * 200;
//    float y = 100;
//
//    return { x, y,
//             (float)m_cardTexture.getSize().x,
//             (float)m_cardTexture.getSize().y };
//}
