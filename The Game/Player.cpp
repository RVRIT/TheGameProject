#include "Player.h"
#include <algorithm>

size_t Player::s_nextId = 1;


Player::Player(std::string name) : m_id(s_nextId++), m_name(name)
{

}

size_t Player::getId() const
{
	return m_id;
}

const std::string& Player::getName() const
{
	return m_name;
}

const std::vector<Card>& Player::getHand() const
{
	return m_hand;
}

size_t Player::getHandSize() const
{
	return m_hand.size();
}

void Player::sortHand()
{
	std::sort(m_hand.begin(), m_hand.end());
}

