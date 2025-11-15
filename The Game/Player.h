#pragma once

#include "Card.h"
#include "Deck.h"
#include <vector>
#include <string>
#include <string_view>
#include <cstddef>

class Player
{
private:
	static size_t s_nextId;
	size_t m_id;
	std::string m_name;
	std::vector<Card> m_hand;

public:
	explicit Player(std::string_view name);

	[[nodiscard]] size_t getId() const noexcept;
	[[nodiscard]] const std::string& getName() const noexcept;
	[[nodiscard]] const std::vector<Card>& getHand() const noexcept;
	[[nodiscard]] size_t getHandSize() const noexcept;

	void sortHand();

	void addCard(const Card& card);

	[[nodiscard]] Card playCard(size_t handIndex);
};