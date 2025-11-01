#include "Card.h"
#include <stdexcept>

Card::Card(uint8_t value) : m_value(value)
{
	if (m_value < 2 || m_value > 99)
	{
		throw std::out_of_range("Valoarea cartii trebuie sa fie intre 2 si 99.");
	}
}

uint8_t Card::getValue() const
{
	return m_value;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << static_cast<int>(card.m_value);
	return os;
}

bool operator<(const Card& cardA, const Card& cardB)
{
	return cardA.m_value < cardB.m_value;
}
