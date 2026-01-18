#pragma once
#include <chrono>
#include "DBManager.h"
#include "Player.h"
#include "Deck.h"
#include "Pile.h"
#include "../CardEngine/Card.h"
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <cstddef>
#include "GameSnapshot.h"


class Game
{
public:
	static constexpr size_t HAND_SIZE_2_PLAYERS = 8;
	static constexpr size_t HAND_SIZE_3_PLAYERS = 7;
	static constexpr size_t HAND_SIZE_4_OR_5_PLAYERS = 6;

	static constexpr size_t MIN_CARDS_TO_PLAY_NORMAL = 2;
	static constexpr size_t MIN_CARDS_TO_PLAY_EMPTY_DECK = 1;

public:
	std::string getCurrentPlayerName() const;
	bool isPlayerTurn(const std::string& playerName) const;
	explicit Game(const std::vector<std::string_view>& PlayerNames);

	// COMENZI PENTRU CLIENT(SFML)

	 bool attemptPlayCard(size_t handIndex, size_t pileIndex);
	 bool attemptEndTurn();


	// DATE PENTRU DESENARE

	 GameSnapshot getSnapshot(const std::string& requestingPlayerName) const;


	// Getteri utili pentru UI

	 bool isGameOver() const;
	 bool isVictory() const;

	size_t getCardsPlayedThisTurn() const;

	size_t getMinCardsRequired() const; // Returns 2 normally, or 1 if the deck is empty (Endgame Rule).

private:
	void setupGame();
	
	void nextTurn();
	void drawCardsForCurrentPlayer();

	bool checkWinCondition() const;
	bool checkLoseCondition() const;
	bool canPlayerMakeAnyMove(const Player& player) const;
	int calculateScore(const Player& player) const;

	Deck m_deck;
	std::vector<Player> m_players;
	std::array<Pile, 4> m_piles;

	size_t m_currentPlayerIndex;
	size_t m_cardsPlayedThisTurn = 0;
	size_t m_initialHandSize;

	bool m_isGameOver = false;
	bool m_playerWon = false;

	std::chrono::steady_clock::time_point m_startTime;
	bool m_statsSaved = false;
	void saveGameResults();
};