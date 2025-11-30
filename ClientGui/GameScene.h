//#pragma once
//#include <SFML/Graphics.hpp>
//#include "Scene.h"
//#include "MockGameManager.h"
//
//class GameScene : public Scene
//{
//public:
//    GameScene(sf::Font& font);
//
//    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
//    void update(sf::Time dt) override;
//    void draw(sf::RenderWindow& window) override;
//
//private:
//    sf::Font& m_font;
//
//    sf::Texture m_cardTexture;
//    sf::Sprite  m_cardSprite;
//
//    GameSnapshot m_snapshot;
//
//    int m_selectedCard = -1;
//
//    sf::FloatRect getCardBounds(int cardIndex) const;
//    sf::FloatRect getPileBounds(int pileIndex) const;
//};
