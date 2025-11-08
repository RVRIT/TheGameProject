#pragma once

#include <vector>
#include "Card.h"
#include "shuffler.hpp"

class Deck
{
public:
	Deck();

	Card drawCard();

	bool isEmpty() const;

	size_t size() const;

private:
	std::vector<Card> m_cards;
	Shuffler<Card> m_shuffler;
};

