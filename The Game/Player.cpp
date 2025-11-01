#include "Player.h"


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

