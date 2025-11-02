#include "shuffler.hpp"
#include <random>
#include <ranges>
#include <span>

namespace game {

    Shuffler::Shuffler() {
        std::random_device rd;
        rng.seed(rd());
    }

    void Shuffler::shuffle(Deck& deck) {
        std::ranges::shuffle(deck, rng);
    }

    auto Shuffler::deal(std::span<const Deck::value_type> deck, std::size_t numPlayers, std::size_t cardsPerPlayer) const -> Hands {
        Hands hands(numPlayers);
        auto it = deck.begin();

        for (std::size_t card = 0; card < cardsPerPlayer && it != deck.end(); ++card) {
            for (std::size_t player = 0; player < numPlayers && it != deck.end(); ++player) {
                hands[player].push_back(*it);
                ++it;
            }
        }

        return hands;
    }

} 