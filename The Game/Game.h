#pragma once

#include "Player.h"
#include "Deck.h"
#include "Pile.h"
#include "Card.h"
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <cstddef>
#include "GameSnapshot.h"


class Game
{
public:
	
	explicit Game(const std::vector<std::string_view>& PlayerNames);

	// COMENZI PENTRU CLIENT(SFML)

	[[nodiscard]] bool attemptPlayCard(size_t handIndex, size_t pileIndex);
	[[nodiscard]] bool attemptEndTurn();


	// DATE PENTRU DESENARE

	[[nodiscard]] GameSnapshot getSnapshot(const std::string& requestingPlayerName) const;


	// Getteri utili pentru UI

	[[nodiscard]] bool isGameOver() const { return m_isGameOver; }
	[[nodiscard]] bool isVictory() const { return m_playerWon; }

	[[nodiscard]] size_t getCardsPlayedThisTurn() const { return m_cardsPlayedThisTurn; }

	[[nodiscard]] size_t getMinCardsRequired() const; // Returns 2 normally, or 1 if the deck is empty (Endgame Rule).

private:
	void setupGame();
	void playTurn();
	void nextTurn() noexcept;
	void drawCardsForCurrentPlayer();

	[[nodiscard]] bool checkWinCondition() const noexcept;
	[[nodiscard]] bool canPlayerMakeAnyMove(const Player& player) const noexcept;


	Deck m_deck;
	std::vector<Player> m_players;
	std::array<Pile, 4> m_piles;

	size_t m_currentPlayerIndex;
	size_t m_cardsPlayedThisTurn = 0;
	size_t m_initialHandSize;

	bool m_isGameOver = false;
	bool m_playerWon = false;
};