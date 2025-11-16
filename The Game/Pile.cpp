#include "Pile.h"

Pile::Pile(PileType type) : m_type{ type }
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

uint8_t Pile::getTopValue() const
{
	return m_topCardValue;
}

bool Pile::canPlace(const Card& card) const noexcept
{
	uint8_t val = card.getValue();

	if (m_type == PileType::ASCENDING)
	{
		if (val > m_topCardValue) return true;
		if (val == m_topCardValue - 10) return true;
	}
	else
	{
		if (val < m_topCardValue) return true;
		if (val == m_topCardValue + 10) return true;
	}
	return false;
}

[[nodiscard]] bool Pile::isTenBackMove(const Card& card) const noexcept
{
	uint8_t val = card.getValue();
	if (m_type == PileType::ASCENDING)
	{
		return val == m_topCardValue - 10;
	}
	else
	{
		return val == m_topCardValue + 10;
	}
}

void Pile::placeCard(const Card& card)
{
	m_topCardValue = card.getValue();
}