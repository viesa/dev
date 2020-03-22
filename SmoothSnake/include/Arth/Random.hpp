#pragma once
#include <stdlib.h>
#include <time.h>

#include "../Core/Library.hpp"

class Random
{

public:
	Random(int const &lower = 0, int const &upper = 10)
		: m_lower(lower),
		  m_upper(upper){};

	int Gen() { return (double)((rand() % ((int)m_upper - (int)m_lower)) + (int)m_lower); }

	void SetLowerBound(int lowerBound_IN) { m_lower = lowerBound_IN; };
	void SetUpperBound(int upperBound_IN) { m_upper = upperBound_IN; };

	static double Number(double lower, double upper)
	{
		srand(time(NULL));
		if (lower > upper)
		{
			std::swap(lower, upper);
		}
		return (double)((rand() % ((int)upper - (int)lower)) + (int)lower);
	}

	static int Integer(int lower, int upper)
	{
		if (lower > upper)
		{
			std::swap(lower, upper);
		}
		return rand() % (upper - lower) + lower;
	}

	static char Character(std::string choices)
	{
		int index = rand() % (choices.length());
		return choices[index];
	}

	template <typename T>
	static T Choices(std::vector<T> choices)
	{
		int index = rand() % (choices.size());
		return choices[index];
	}

	static sf::Vector2f UnitVector2f()
	{
		float x = rand() % 1000;
		float y = rand() % 1000;
		return vf::Unit(sf::Vector2f(x, y));
	}

private:
	int m_lower;
	int m_upper;
};