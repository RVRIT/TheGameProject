#pragma once

#include <vector>
#include <random>
#include <cstddef> 
#include <ranges>

template <typename T>

class Shuffler
{
public:
	Shuffler();

	void shuffle(std::vector<T>& items);

private:
	std::mt19937 m_rng;

};

template <typename T>
Shuffler<T>::Shuffler()
{
	std::random_device rd;
	m_rng.seed(rd());
}

template<typename T>
void Shuffler<T>::shuffle(std::vector<T>& items)
{
	std::ranges::shuffle(items, m_rng);

}