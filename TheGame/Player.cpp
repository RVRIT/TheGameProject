#include "Player.h"
#include <algorithm>
#include <stdexcept>
#include <ranges>

size_t Player::s_nextId = 1;

Player::Player(std::string_view name)
	: m_id(s_nextId++), m_name(name)
{
}

size_t Player::getId() const noexcept
{
	return m_id;
}

const std::string& Player::getName() const noexcept
{
	return m_name;
}

const std::vector<Card>& Player::getHand() const noexcept
{
	return m_hand.getCards();
}

size_t Player::getHandSize() const noexcept
{
	return m_hand.count();
}


std::vector<Player::Move> Player::findPossibleMoves(
	const std::array<Pile, 4>& piles) const
{
	std::vector<Move> possibleMoves;

	const auto& handVector = m_hand.getCards();

	for (size_t cardIdx = 0; cardIdx < handVector.size(); ++cardIdx)
	{
		for (size_t pileIdx = 0; pileIdx < piles.size(); ++pileIdx)
		{
			if (piles[pileIdx].canPlace(handVector[cardIdx]))
			{
				possibleMoves.emplace_back(cardIdx, pileIdx);
			}
		}
	}
	return possibleMoves;
}

void Player::sortHand()
{
	m_hand.sort();
}

void Player::addCard(const Card& card)
{
	m_hand.add(card);
}

Card Player::playCard(size_t handIndex)
{
	return m_hand.removeAt(handIndex);
}