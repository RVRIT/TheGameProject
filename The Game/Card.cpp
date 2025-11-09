#include "Card.h"
#include <stdexcept>
#include <format> 

constexpr Card::Card(uint8_t value) : m_value(value)
{
	if (m_value < 2 || m_value > 99)
	{
		throw std::out_of_range(
			std::format("Valoarea cartii {} este invalida. Trebuie sa fie intre 2 si 99.",
				static_cast<int>(m_value))
		);
	}
}

[[nodiscard]] constexpr uint8_t Card::getValue() const noexcept
{
	return m_value;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << static_cast<int>(card.getValue());
	return os;
}