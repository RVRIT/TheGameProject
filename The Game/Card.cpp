#include "Card.h"

Card::Card(uint8_t value) : m_value(value)
{

}

uint8_t Card::getValue() const
{
	return m_value;
}
