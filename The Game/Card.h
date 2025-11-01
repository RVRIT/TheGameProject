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
};

