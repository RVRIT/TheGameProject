#pragma once
#include <iostream>
#include <cstdint>


class Card
{
private:
	uint8_t m_value;

public:
	Card(uint8_t value);

	uint8_t getValue() const;

	friend std::ostream& operator<<(std::ostream& os, const Card& card);

	friend bool operator<(const Card& cardA, const Card& cardB);
};

