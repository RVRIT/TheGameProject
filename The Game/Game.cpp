#include "Game.h"
#include <iostream>
#include <stdexcept>
#include <format>
#include <string>
#include <limits>
#include <algorithm>
#include <ranges>

Game::Game(const std::vector<std::string_view>& playerNames)
	: m_piles{
		Pile(PileType::ASCENDING),
		Pile(PileType::ASCENDING),
		Pile(PileType::DESCENDING),
		Pile(PileType::DESCENDING)
	},
	m_currentPlayerIndex(0),
	m_minCardsToPlayPerTurn(2),
	m_initialHandSize(0),
	m_isGameOver(false)
{
	if (playerNames.empty() || playerNames.size() > 5)
	{
		throw std::invalid_argument("Numar invalid de jucatori. Trebuie sa fie intre 1 si 5.");
	}

	m_players.reserve(playerNames.size());
	for (const auto& name : playerNames)
	{
		m_players.emplace_back(name);
	}

	setupGame();
}

void Game::setupGame()
{
	switch (m_players.size())
	{
	case 1: m_initialHandSize = 8; break;
	case 2: m_initialHandSize = 7; break;
	default: m_initialHandSize = 6; break;
	}

	for (size_t i = 0; i < m_initialHandSize; ++i)
	{
		for (auto& player : m_players)
		{
			if (!m_deck.isEmpty())
			{
				Card drawnCard = m_deck.drawCard();

				player.addCard(drawnCard);
			}
		}
	}

	for (auto& player : m_players)
	{
		player.sortHand();
	}
}

void Game::displayGameState() const
{
	std::cout << std::format("\n--- Deck: {} cards left ---\n", m_deck.size());

	std::cout << "Piles:\n";

	for (size_t i = 0; i < m_piles.size(); ++i)
	{
		const char* typeStr = (m_piles[i].getType() == PileType::ASCENDING) ? "ASC " : "DESC ";

		std::cout << std::format("  {}: {}  - {}\n", i, typeStr, static_cast<int>(m_piles[i].getTopValue()));
	}

	const auto& currentPlayer = m_players[m_currentPlayerIndex];

	std::cout << std::format("\n--- It's {}'s turn ---\n", currentPlayer.getName());

	std::cout << "Your hand:\n";

	const auto& hand = currentPlayer.getHand();

	for (size_t i = 0; i < hand.size(); ++i)
	{
		
		std::cout << std::format("  {}: [{}]\n", i, static_cast<int>(hand[i].getValue()));
	}

	std::cout << "----------------------------------------\n";
}

void Game::nextTurn() noexcept
{
	m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
}

void Game::handlePlayerInput(Player& currentPlayer, size_t& cardsPlayedThisTurn)
{
	while (true)
	{
		std::cout << "Choose a card to play (index): ";
		size_t cardIndex, pileIndex;
		std::cin >> cardIndex;

		std::cout << "Choose a pile (index): ";
		std::cin >> pileIndex;

		if (std::cin.fail() || cardIndex >= currentPlayer.getHandSize()
			|| pileIndex >= m_piles.size())
		{
			std::cerr << "Invalid input.Please try again.\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		const Card& selectedCard = currentPlayer.getHand().at(cardIndex);

		if (m_piles[pileIndex].canPlace(selectedCard))
		{
			Card cardToPlay = currentPlayer.playCard(cardIndex);
			m_piles[pileIndex].placeCard(cardToPlay);
			cardsPlayedThisTurn++;
			break;
		}
		else
		{
			std::cerr << "Invalid move! The card cannot be placed on this pile.\n";
		}
	}
}
