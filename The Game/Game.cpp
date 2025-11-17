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

void Game::displayGameState() const
{
	//trebuie sa se construiasca un obiect cu toate informatiile de display pentru a fi returnate dupa
	// functia nu poate fi void
	// return topvalue la fiecare pile (+lungimea piles pentru afisare front end), hand ul la fiecare player
	// a cui e tura, size-ul la cartile de tras + ultima carte de sus ca sa o poti afisa
	// si daca exista move de facut la fiecare player (True, False) -> map cu fiecare player si state-ul lui
	// (daca mai poate juca sau nu) -> pentru a putea afisa in UI cine joaca si cine nu
	// VEDEM LA CHECK IN DE FAZA CU MAP CU STATE UL PLAYERILOR

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

//delete this bro
void Game::aiPlayTurn(Player& currentPlayer, size_t& cardsPlayedThisTurn)
{
	std::cout << std::format("Este tura lui {} (AI). AI-ul se gândește...\n",
		currentPlayer.getName());

	std::this_thread::sleep_for(std::chrono::seconds(2));

	size_t minCards = (m_deck.isEmpty()) ? 1 : m_minCardsToPlayPerTurn;

	while (cardsPlayedThisTurn < minCards)
	{
		auto moves = currentPlayer.findPossibleMoves(m_piles);
		if (moves.empty())
		{
			break;
		}

		std::optional<Player::Move> bestMove;
		int bestScore = -1;

		for (const auto& move : moves)
		{
			size_t cardIdx = move.first;
			size_t pileIdx = move.second;
			const Card& card = currentPlayer.getHand()[cardIdx];
			const Pile& pile = m_piles[pileIdx];

			int currentScore = 0;
			if (pile.isTenBackMove(card))
			{
				currentScore = 100;
			}
			else
			{
				int diff = std::abs(static_cast<int>(card.getValue()) -
					static_cast<int>(pile.getTopValue()));
				currentScore = 50 - diff;
			}

			if (currentScore > bestScore)
			{
				bestScore = currentScore;
				bestMove = move;
			}
		}

		if (bestMove)
		{
			size_t cardIdx = bestMove->first;
			size_t pileIdx = bestMove->second;

			Card cardToPlay = currentPlayer.getHand()[cardIdx];

			currentPlayer.playCard(cardIdx);
			m_piles[pileIdx].placeCard(cardToPlay);
			cardsPlayedThisTurn++;

			std::cout << std::format("AI-ul {} a jucat [{}] pe pachetul {}.\n",
				currentPlayer.getName(), cardToPlay, pileIdx);

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else
		{
			break;
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

	displayGameState();

	size_t cardsPlayedThisTurn = 0;

	// se scoate asta cu AI-ul
	if (currentPlayer.isAI())
	{
		aiPlayTurn(currentPlayer, cardsPlayedThisTurn);
	}
	else
	{
		humanPlayTurn(currentPlayer, cardsPlayedThisTurn);
	}

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