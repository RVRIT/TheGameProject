#pragma once
#ifdef CARDENGINE_EXPORTS
#define CARD_API __declspec(dllexport)
#else
#define CARD_API __declspec(dllimport)
#endif

#include "Card.h"
#include <vector>
#include <stdexcept>


class CARD_API CardCollection
{
protected:
    std::vector<Card> m_cards;

public:

    CardCollection();

    virtual ~CardCollection();

    CardCollection(const CardCollection& other);

    void add(const Card& c);
    Card removeLast();
    Card removeAt(size_t index);
    size_t count() const;
    void sort();
    bool isEmpty() const;
    void clear();

    void shuffle();

    // modifiable

    std::vector<Card>& getData() { return m_cards; }
    
    // non modifiable
    const std::vector<Card>& getCards() const { return m_cards; }
};