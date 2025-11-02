#pragma once
#include <cstdint>
#include <vector>
#include <compare>
#include <stdexcept>
namespace game {
	struct Card{
	
		std::uint8_t value;
		constexpr Card(std::uint8_t v) :value(v)
		{
			if (v < 2 || v > 99) throw std::out_of_range("Card must be 2–99");
		}
	
	
	};


}
