#pragma once

#include <vector>
#include "../CardEngine/CardCollection.h"
#include "Shuffler.h"

class Deck : public CardCollection
{
public:
	Deck();
	Card drawCard();

	size_t size() const;
};

