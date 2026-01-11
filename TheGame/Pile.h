#pragma once

#include "../CardEngine/Card.h"
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

	 bool canPlace(const Card& card) const noexcept;
	 bool isTenBackMove(const Card& card) const noexcept;
	 int getCardCount() const;


	void placeCard(const Card& card);

public:
	static constexpr uint8_t START_VALUE_ASC = 1;
	static constexpr uint8_t START_VALUE_DESC = 100;
	static constexpr uint8_t STEP_BACK_RULE = 10;

private:
	PileType m_type;
	uint8_t m_topCardValue;

	int m_cardCount;
};