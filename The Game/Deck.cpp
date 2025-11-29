#include "Deck.h"
#include <stdexcept>

Deck::Deck()
{
	for (uint8_t i = 2; i <= 99; ++i)
	{
		this->add(Card(i));
	}

	this->shuffle();

}

Card Deck::drawCard()
{
	return this->removeLast();
}

size_t Deck::size() const
{
	return this->count(); 
}