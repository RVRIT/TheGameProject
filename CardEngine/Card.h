#pragma once
#ifdef CARDENGINE_EXPORTS
#define CARD_API __declspec(dllexport)
#else
#define CARD_API __declspec(dllimport)
#endif

#include <cstdint>
#include <compare>
#include <iostream>

class CARD_API Card
{
private:
    uint8_t m_value;

public:

    Card();
    explicit Card(uint8_t value);
    uint8_t getValue() const noexcept;


    auto operator<=>(const Card& other) const = default;
public:

    static constexpr uint8_t MIN_CARD_VALUE = 2;
    static constexpr uint8_t MAX_CARD_VALUE = 99;
};

CARD_API std::ostream& operator<<(std::ostream& os, const Card& card);