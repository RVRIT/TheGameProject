#pragma once


import <iostream>;
import <vector>;
import <random>;


namespace game
{
	class Shuffler
	{
	public:

		using Deck = std::vector<int>;
		using Hands = std::vector<Deck>;

		Shuffler();

		void shuffle(Deck& deck);

		Hands deal(const Deck& deck, std::size_t numPlayers, std::size_t cardsPerPlayer);



	private:

	std::mt19937 rng;
	};
}


