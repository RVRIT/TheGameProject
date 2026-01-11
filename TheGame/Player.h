#pragma once

#include "../CardEngine/Card.h"
#include "../CardEngine/CardCollection.h"

#include "Deck.h"
#include "Pile.h"
#include <vector>
#include <string>
#include <string_view>
#include <cstddef>
#include <array>

class Player
{
private:
	static size_t s_nextId;
	size_t m_id;
	std::string m_name;
	CardCollection m_hand;
	

public:
	explicit Player(std::string_view name);

	size_t getId() const noexcept;
	const std::string& getName() const noexcept;
	const std::vector<Card>& getHand() const noexcept;
	size_t getHandSize() const noexcept;
	

	using Move = std::pair<size_t, size_t>;
	std::vector<Move> findPossibleMoves(
		const std::array<Pile, 4>& piles) const;

	void sortHand();
	void addCard(const Card& card);
	Card playCard(size_t handIndex);
};