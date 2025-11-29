#pragma once

#include <vector>
#include "../CardEngine/CardCollection.h"
#include "Shuffler.h"

class Deck : public CardCollection
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

