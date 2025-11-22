#include "Game.h"
#include <iostream>
#include <stdexcept>
#include <format>
#include <string>
#include <limits>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <thread>
#include <optional>

//@TODO:
/* - read comments at DisplayGameState method
   - completely delete AI functionalities (nu folosim asa ceva)
   - eliminam functia de skip la turn, maxim se termina jocul cand nu mai poti juca carti
   sau se scoate player-ul respectiv din joc in functie de cum ni se permite
   - sa se adauge functie de calculat scor, return scor:  se incepe cu un scor de 100 de puncte si se pierd
   puncte per fiecare carte nejucata la final de joc sau la eliminarea jucatorului X
   in caz ca ni se permite sa continuam jocul dupa ce un player a fost eliminat scorul se calculeaza astfel:
     scor = 100 - carti_in_mana - (carti_in_teanc / numar_de_jucatori)
	 si gamestate intoarce lista de jucatori in ordinea scorurilor (locul 1, 2, 3 si scorul lor)
*/
Game::Game(const std::vector<std::string_view>& playerNames)
	: m_piles{
		Pile(PileType::ASCENDING),
		Pile(PileType::ASCENDING),
		Pile(PileType::DESCENDING),
		Pile(PileType::DESCENDING)
	},
	m_currentPlayerIndex(0),
	m_cardsPlayedThisTurn(0),
	m_initialHandSize(0),
	m_isGameOver(false),
	m_playerWon(false)
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

bool Game::attemptPlayCard(size_t handIndex, size_t pileIndex)
{
	if (m_isGameOver) return false;
	if (pileIndex >= m_piles.size()) return false;

	Player& currentPlayer = m_players[m_currentPlayerIndex];

	if (handIndex >= currentPlayer.getHandSize()) return false;

	const Card& cardToPlay = currentPlayer.getHand()[handIndex];

	if (m_piles[pileIndex].canPlace(cardToPlay))
	{
		Card played = currentPlayer.playCard(handIndex);
		m_piles[pileIndex].placeCard(played);

		m_cardsPlayedThisTurn++;


		if (checkWinCondition())
		{
			m_isGameOver = true;
			m_playerWon = true;
		}


		return true;
	}
	return false;
}

bool Game::attemptEndTurn()
{
	if (m_isGameOver) return false;

	size_t minRequired = getMinCardsRequired();
	if (m_cardsPlayedThisTurn < minRequired)
	{
		return false;
	}

	drawCardsForCurrentPlayer();

	if (checkWinCondition())
	{
		m_isGameOver = true;
		m_playerWon = true;
		return true;
	}

	nextTurn();

	Player& nextPlayer = m_players[m_currentPlayerIndex];
	if (!canPlayerMakeAnyMove(nextPlayer))
	{
		m_isGameOver = true;
		m_playerWon = false;
	}
	return true;

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



void Game::nextTurn() noexcept
{
	m_cardsPlayedThisTurn = 0; // resetam 
	m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
}



void Game::drawCardsForCurrentPlayer()
{
	// functia asta o sa iti traga un singur card (ultimul din varf) la fiecare click, nu primeste
	// cardsToDraw ca parametru

	Player& player = m_players[m_currentPlayerIndex];

	size_t cardsToDraw = m_cardsPlayedThisTurn;
	for (size_t i = 0; i < cardsToDraw; ++i)
	{
		if (m_deck.isEmpty())
		{
			break;
		}
		player.addCard(m_deck.drawCard());
	}
	player.sortHand();
}


[[nodiscard]] bool Game::canPlayerMakeAnyMove(const Player& player) const noexcept
{
	return !player.findPossibleMoves(m_piles).empty();
}

size_t Game::getMinCardsRequired() const // Returns 2 normally, or 1 if the deck is empty (Endgame Rule).
{
	return m_deck.isEmpty() ? 1 : 2;
}

[[nodiscard]] bool Game::checkWinCondition() const noexcept
{
	if (m_deck.isEmpty())
	{
		bool allHandsEmpty = std::ranges::all_of(m_players,
			[](const Player& p) { return p.getHandSize() == 0; }
		);

		if (allHandsEmpty)
		{
			return true;
		}
	}
	return false;
}

GameSnapshot Game::getSnapshot(const std::string& requestingPlayerName) const
{
	GameSnapshot snap;

	for (size_t i = 0;i < m_piles.size();++i)
	{
		PileInfo info;
		info.id = static_cast<int>(i);
		info.type = (m_piles[i].getType() == PileType::ASCENDING) ? "ASC" : "DESC";
		info.topValue = m_piles[i].getTopValue();

		snap.piles.push_back(info);

	}

	snap.deckSize = static_cast<int>(m_deck.size());

	for (size_t i = 0;i < m_players.size();++i)
	{
		const Player& p = m_players[i];

		bool canMove = canPlayerMakeAnyMove(p);
		bool isTurn = (i == m_currentPlayerIndex);	

		if (p.getName() == requestingPlayerName)
		{
			for (const auto& card : p.getHand())
			{
				snap.myHand.push_back(card.getValue());
			}

			// Temporary placeholder: 'cardsPlayed' is currently a local variable in the run loop. 
			// Will be connected to a class member variable when switching to Server architecture.
			snap.cardsPlayedThisTurn = 0; 
			snap.minCardsToPlay = getMinCardsRequired();
		}
		else
		{
			OtherPlayerInfo other;
			other.name = p.getName();
			other.cardCount = static_cast<int>(p.getHandSize());
			other.canMakeMove = canMove;
			other.isCurrentPlayer = isTurn;

			snap.opponents.push_back(other);
		}
	}

	snap.isGameOver = m_isGameOver;
	snap.playerWon = m_playerWon;

	snap.message = "";						// Empty message for now

	return snap;

}
