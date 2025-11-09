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

private:
	PileType m_type;
	uint8_t m_topCardValue;

};

