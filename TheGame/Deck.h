#pragma once

#include <vector>
#include "Shuffler.h"
#include "CardCollection.h"

class Deck : public CardCollection
{
public:
	Deck();
	Card drawCard();

	size_t size() const;
};

