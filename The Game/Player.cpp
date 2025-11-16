#include "Player.h"
#include <algorithm>
#include <stdexcept>
#include <ranges>

size_t Player::s_nextId = 1;

Player::Player(std::string_view name, bool isAI)
	: m_id(s_nextId++), m_name(name), m_isAI(isAI)
{
}

[[nodiscard]] size_t Player::getId() const noexcept
{
	return m_id;
}

[[nodiscard]] const std::string& Player::getName() const noexcept
{
	return m_name;
}

[[nodiscard]] const std::vector<Card>& Player::getHand() const noexcept
{
	return m_hand;
}

[[nodiscard]] size_t Player::getHandSize() const noexcept
{
	return m_hand.size();
}

[[nodiscard]] bool Player::isAI() const noexcept
{
	return m_isAI;
}

[[nodiscard]] std::vector<Player::Move> Player::findPossibleMoves(
	const std::array<Pile, 4>& piles) const
{
	std::vector<Move> possibleMoves;
	for (size_t cardIdx = 0; cardIdx < m_hand.size(); ++cardIdx)
	{
		for (size_t pileIdx = 0; pileIdx < piles.size(); ++pileIdx)
		{
			if (piles[pileIdx].canPlace(m_hand[cardIdx]))
			{
				possibleMoves.emplace_back(cardIdx, pileIdx);
			}
		}
	}
	return possibleMoves;
}

void Player::sortHand()
{
	std::ranges::sort(m_hand);
}

void Player::addCard(const Card& card)
{
	m_hand.push_back(card);
}

[[nodiscard]] Card Player::playCard(size_t handIndex)
{
	if (handIndex >= m_hand.size())
	{
		throw std::out_of_range("Index invalid pentru mâna jucătorului.");
	}

	Card cardToPlay = m_hand[handIndex];
	m_hand.erase(m_hand.begin() + handIndex);
	return cardToPlay;
}