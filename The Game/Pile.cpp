#include "Pile.h"

Pile::Pile(PileType type) : m_type{type}
{
	if (m_type == PileType::ASCENDING)
	{
		m_topCardValue = 1;
	}
	else
	{
		m_topCardValue = 100;
	}
}

PileType Pile::getType() const
{
	return m_type;
}
