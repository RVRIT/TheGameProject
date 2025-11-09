#pragma once

#include "Player.h"
#include "Deck.h"
#include "Pile.h"
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <cstddef>

class Game
{
public:
	explicit Game(const std::vector<std::string_view>& playerNames);

	void run();

private:
	void setupGame();
	void playTurn();
	void displayGameState() const;
	void nextTurn() noexcept;

	[[nodiscard]] bool checkWinCondition() const noexcept;
	[[nodiscard]] bool canPlayerMakeAnyMove(const Player& player) const noexcept;
	void handlePlayerInput(Player& currentPlayer, size_t& cardsPlayedThisTurn);

	Deck m_deck;
	std::vector<Player> m_players;
	std::array<Pile, 4> m_piles;

	size_t m_currentPlayerIndex;
	size_t m_minCardsToPlayPerTurn;
	size_t m_initialHandSize;
	bool m_isGameOver;
};