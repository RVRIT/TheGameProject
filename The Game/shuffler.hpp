#pragma once

#include <vector>
#include <random>
#include <cstddef> 
#include <span>

namespace game {

    class Shuffler {
    public:
        using Deck = std::vector<int>; 
        using Hands = std::vector<Deck>;

        Shuffler();

        void shuffle(Deck& deck);

        Hands deal(std::span<const Deck::value_type> deck, std::size_t numPlayers, std::size_t cardsPerPlayer) const;

    private:
        mutable std::mt19937 rng;
    };

} 