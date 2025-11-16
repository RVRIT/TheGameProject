#pragma once

#include "Card.h"
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
	std::vector<Card> m_hand;
	bool m_isAI;

public:
	explicit Player(std::string_view name, bool isAI = false);

	[[nodiscard]] size_t getId() const noexcept;
	[[nodiscard]] const std::string& getName() const noexcept;
	[[nodiscard]] const std::vector<Card>& getHand() const noexcept;
	[[nodiscard]] size_t getHandSize() const noexcept;
	[[nodiscard]] bool isAI() const noexcept;

	using Move = std::pair<size_t, size_t>;
	[[nodiscard]] std::vector<Move> findPossibleMoves(
		const std::array<Pile, 4>& piles) const;

	void sortHand();
	void addCard(const Card& card);
	[[nodiscard]] Card playCard(size_t handIndex);
};