#include "Card.h"
#include <stdexcept>
#include <format>

Card::Card() :m_value(0)
{

}

Card::Card(uint8_t value)
	:m_value(value)
{
	if (m_value < 2 || m_value>99)
	{
		throw std::out_of_range(
			std::format("The value of the book {} is invalid. It must be between 2 and 99.", 
			static_cast<int>(m_value))
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

