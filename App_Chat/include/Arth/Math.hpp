#pragma once
#include <cmath>

namespace Constants
{
constexpr double PI_D = 3.141592653589793238462643383279;
constexpr float PI = float(PI_D);
} // namespace Constants

template <typename T>
auto sq(T x)
{
	return x * x;
}