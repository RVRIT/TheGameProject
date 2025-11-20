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
Game::Game(const std::vector<PlayerConfig>& playerConfigs)
	: m_piles{
		Pile(PileType::ASCENDING),
		Pile(PileType::ASCENDING),
		Pile(PileType::DESCENDING),
		Pile(PileType::DESCENDING)
	},
	m_currentPlayerIndex(0),
	m_minCardsToPlayPerTurn(2),
	m_initialHandSize(0),
	m_isGameOver(false),
	m_playerWon(false)
{
	if (playerConfigs.empty() || playerConfigs.size() > 5)
	{
		throw std::invalid_argument("Numar invalid de jucatori. Trebuie sa fie intre 1 si 5.");
	}

	m_players.reserve(playerConfigs.size());
	for (const auto& config : playerConfigs)
	{
		m_players.emplace_back(config.first, config.second);
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



void Game::nextTurn() noexcept
{
	// verifica daca mai e in joc sau nu, si daca nu da skip
	m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
}

void Game::humanPlayTurn(Player& currentPlayer, size_t& cardsPlayedThisTurn)
{
	bool turnFinished = false;
	while (!turnFinished)
	{
		// nu exista functia de a da "skip" la tura, poti doar sa tragi sau sa pui carte
		std::cout << "Alege o carte (index) sau 'p' pentru a pasa: ";
		//cardInput vine ca parametru din UI, ca index -> int, size_t whatever
		std::string cardInput;
		std::cin >> cardInput;

		if (cardInput == "p" || cardInput == "P")
		{
			turnFinished = true;
			continue;
		}

		size_t cardIndex;
		try
		{
			cardIndex = std::stoull(cardInput);
		}
		catch (const std::exception&)
		{
			std::cerr << "Input invalid. Introdu un număr sau 'p'.\n";
			continue;
		}

		// si pileIndex o sa vina din UI (se apasa pe pile)
		std::cout << "Alege un pachet (index): ";
		size_t pileIndex;
		std::cin >> pileIndex;


		if (std::cin.fail() || cardIndex >= currentPlayer.getHandSize()
			|| pileIndex >= m_piles.size())
		{
			std::cerr << "Input invalid. Te rog reîncearcă.\n";
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

			std::cout << std::format("Ai jucat [{}] pe pachetul {}.\n",
				cardToPlay, pileIndex);

			std::cout << "Mâna ta actualizată:\n";
			const auto& hand = currentPlayer.getHand();
			for (size_t i = 0; i < hand.size(); ++i)
			{
				std::cout << std::format("  {}: [{}]\n", i, hand[i]);
			}
		}
		else
		{
			std::cerr << "Mutare invalidă! Cartea nu poate fi pusă pe acest pachet.\n";
		}
	}
}



void Game::drawCardsForPlayer(Player& player, size_t cardsToDraw)
{
	// functia asta o sa iti traga un singur card (ultimul din varf) la fiecare click, nu primeste
	// cardsToDraw ca parametru
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

int Game::getMinCardsRequired() const // Returns 2 normally, or 1 if the deck is empty (Endgame Rule).
{
	return m_deck.isEmpty() ? 1 : static_cast<int>(m_minCardsToPlayPerTurn);
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

void Game::playTurn()
{
	Player& currentPlayer = m_players[m_currentPlayerIndex];

	if (!canPlayerMakeAnyMove(currentPlayer))
	{
		std::cout << std::format("\n!!!! GAME OVER !!!!\n");
		std::cout << std::format("Jucătorul {} nu mai poate face nicio mutare.\n",
			currentPlayer.getName());
		m_isGameOver = true;
		m_playerWon = false;
		return;
	}

	

	size_t cardsPlayedThisTurn = 0;

	
	humanPlayTurn(currentPlayer, cardsPlayedThisTurn);
	

	size_t minCardsRequired = (m_deck.isEmpty()) ? 1 : m_minCardsToPlayPerTurn;

	if (cardsPlayedThisTurn < minCardsRequired)
	{
		if (cardsPlayedThisTurn == 0 && canPlayerMakeAnyMove(currentPlayer))
		{
			std::cout << std::format("\n!!!! GAME OVER !!!!\n");
			std::cout << std::format("Jucătorul {} a pasat, dar mai avea mutări.\n",
				currentPlayer.getName());
			m_isGameOver = true;
			m_playerWon = false;
			return;
		}
	}

	drawCardsForPlayer(currentPlayer, cardsPlayedThisTurn);

	if (checkWinCondition())
	{
		std::cout << std::format("\n****** FELICITĂRI! ******\n");
		std::cout << "Ați câștigat jocul! Pachetul și toate mâinile sunt goale!\n";
		m_isGameOver = true;
		m_playerWon = true;
		return;
	}

	nextTurn();
}

void Game::run()
{
	std::cout << "Bun venit la THE GAME!\n";
	std::cout << "Reguli: Jucați cel puțin " << m_minCardsToPlayPerTurn
		<< " cărți pe tur.\n";
	std::cout << "Pachetele ASC (1) cresc, pachetele DESC (100) descresc.\n";
	std::cout << "Puteți juca o carte cu exact 10 mai mică (pe ASC) sau 10 mai mare (pe DESC) pentru a 'reseta' pachetul.\n";

	while (!m_isGameOver)
	{
		playTurn();
	}

	std::cout << "\n--- STATISTICI FINALE ---\n";
	std::cout << "Rezultat: " << (m_playerWon ? "Victorie!" : "Înfrângere") << "\n";
	std::cout << "Cărți rămase în pachet: " << m_deck.size() << "\n";
	size_t totalCardsInHand = 0;
	for (const auto& player : m_players)
	{
		totalCardsInHand += player.getHandSize();
	}
	std::cout << "Total cărți în mâinile jucătorilor: " << totalCardsInHand << "\n";
	std::cout << "---------------------------\n";
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
