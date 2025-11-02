#include "shuffler.hpp"


namespace game
{
	Shuffler::Shuffler()
	{
		std::random_device rd;
		rng.seed(rd());
	}
	
	void Shuffler::shuffle(Deck& deck)
	{
		std::ranges::shuffle(deck, rng);
	}
}