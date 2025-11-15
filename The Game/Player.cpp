#include "Player.h"
#include <algorithm>
#include <stdexcept>
#include <ranges>

size_t Player::s_nextId = 1;

Player::Player(std::string_view name)
	: m_id(s_nextId++), m_name(name)
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

	m_hand[handIndex] = m_hand.back();
	m_hand.pop_back();

	return cardToPlay;
}