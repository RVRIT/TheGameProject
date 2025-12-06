#include "CardCollection.h"
#include "Shuffler.h"
#include <algorithm>

CardCollection::CardCollection() = default;

CardCollection::~CardCollection() = default;

CardCollection::CardCollection(const CardCollection& other) : m_cards{ other.m_cards }
{
}

void CardCollection::add(const Card& c)
{
    m_cards.push_back(c);
}

Card CardCollection::removeLast() 
{
    if (m_cards.empty()) throw std::runtime_error("Collection empty");
    Card c = m_cards.back();
    m_cards.pop_back();
    return c;
}

Card CardCollection::removeAt(size_t index) 
{
    if (index >= m_cards.size()) throw std::runtime_error("Index out of range");
    Card c = m_cards[index];
    m_cards.erase(m_cards.begin() + index);
    return c;
}

size_t CardCollection::count() const
{
    return m_cards.size();
}
bool CardCollection::isEmpty() const 
{
    return m_cards.empty();
}
void CardCollection::clear() 
{
    m_cards.clear();
}

void CardCollection::shuffle()
{
    Shuffler<Card> internalShuffler;

    internalShuffler.shuffle(m_cards);
}

void CardCollection::sort()
{
    std::sort(m_cards.begin(), m_cards.end());
}