#include "Pile.h"

Pile::Pile(PileType type) : m_type{ type }, m_cardCount{0}
{
	m_topCardValue = (m_type == PileType::ASCENDING) ? START_VALUE_ASC : START_VALUE_DESC;
}

PileType Pile::getType() const
{
	return m_type;
}

uint8_t Pile::getTopValue() const
{
	return m_topCardValue;
}

bool Pile::canPlace(const Card& card) const noexcept {
	uint8_t val = card.getValue();
	if (m_type == PileType::ASCENDING) {
		if (val > m_topCardValue) return true;
		if (val == m_topCardValue - STEP_BACK_RULE) return true;
	}
	else {
		if (val < m_topCardValue) return true;
		if (val == m_topCardValue + STEP_BACK_RULE) return true;
	}
	return false;
}

bool Pile::isTenBackMove(const Card& card) const noexcept
{
	uint8_t val = card.getValue();
	if (m_type == PileType::ASCENDING)
	{
		return val == m_topCardValue - STEP_BACK_RULE; 
	}
	else
	{
		return val == m_topCardValue + STEP_BACK_RULE;
	}
}

int Pile::getCardCount() const
{
	return m_cardCount;
}

void Pile::placeCard(const Card& card)
{
	m_topCardValue = card.getValue();
	m_cardCount++;
}