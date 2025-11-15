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