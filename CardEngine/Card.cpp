#include "Card.h"
#include <stdexcept>
#include <format>

Card::Card() :m_value(0)
{

}

Card::Card(uint8_t value)
	:m_value(value)
{
	if (m_value < MIN_CARD_VALUE || m_value> MAX_CARD_VALUE)
	{
		throw std::out_of_range(
			std::format("The value is invalid. It must be between {} and {}.",
				static_cast<int>(MIN_CARD_VALUE), static_cast<int>(MAX_CARD_VALUE))
		);
	}
}
 uint8_t Card::getValue() const noexcept
{
	return m_value;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << static_cast<int>(card.getValue());
	return os;
}

