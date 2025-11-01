#pragma once

#include "Card.h"
#include <vector>
#include <string>
#include <cstddef>

class Player
{
private:
	static size_t s_nextId;
	size_t m_id;
	std::string m_name;
	std::vector<Card> m_hand;

public:
	Player(std::string name);

	size_t getId() const;
	const std::string& getName() const;
	const std::vector<Card>& getHand() const;
	size_t getHandSize() const;

	void sortHand();

};

