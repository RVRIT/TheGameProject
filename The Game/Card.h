#pragma once
#include <iostream>
#include <cstdint>
#include <compare> 

class Card
{
private:
	uint8_t m_value;

public:
	explicit constexpr Card(uint8_t value);

	[[nodiscard]] constexpr uint8_t getValue() const noexcept;

	constexpr auto operator<=>(const Card& other) const noexcept = default;
};

std::ostream& operator<<(std::ostream& os, const Card& card);