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
	using PlayerConfig = std::pair<std::string_view, bool>;
	explicit Game(const std::vector<PlayerConfig>& playerConfigs);

	void run();

	[[nodiscard]] GameSnapshot getSnapshot(const std::string& requestingPlayerName) const;

private:
	void setupGame();
	void playTurn();
	void nextTurn() noexcept;

	[[nodiscard]] bool checkWinCondition() const noexcept;
	[[nodiscard]] bool canPlayerMakeAnyMove(const Player& player) const noexcept;

	[[nodiscard]] int getMinCardsRequired() const; // Returns 2 normally, or 1 if the deck is empty (Endgame Rule).


	void humanPlayTurn(Player& currentPlayer, size_t& cardsPlayedThisTurn);
	
	void drawCardsForPlayer(Player& player, size_t cardsToDraw);

	Deck m_deck;
	std::vector<Player> m_players;
	std::array<Pile, 4> m_piles;

	size_t m_currentPlayerIndex;
	size_t m_minCardsToPlayPerTurn;
	size_t m_initialHandSize;
	bool m_isGameOver;
	bool m_playerWon;
};