#pragma once

#include "Card.h"
#include <cstdint>

enum class PileType
{
	ASCENDING,
	DESCENDING
};

class Pile
{
public:
	Pile(PileType type);

	PileType getType() const;

	uint8_t getTopValue() const;

	[[nodiscard]] bool canPlace(const Card& card) const noexcept;
	[[nodiscard]] bool isTenBackMove(const Card& card) const noexcept;
	[[nodiscard]] int getCardCount() const;


	void placeCard(const Card& card);

private:
	PileType m_type;
	uint8_t m_topCardValue;

	int m_cardCount;
};