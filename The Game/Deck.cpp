#include "Deck.h"
#include <stdexcept>

Deck::Deck()
{
	m_cards.reserve(98);

	for(uint8_t i = 2;i <= 99;++i)
	{ 
		m_cards.emplace_back(i);
	}

	m_shuffler.shuffle(m_cards);

}

Card Deck::drawCard()
{
	if (isEmpty())
	{
		throw std::runtime_error("Cannot draw from an empty deck.");
	}

	Card topCard = m_cards.back();
	m_cards.pop_back();

	return topCard;
}

bool Deck::isEmpty() const
{
	return m_cards.empty();
}

size_t Deck::size() const
{
	return m_cards.size();
}