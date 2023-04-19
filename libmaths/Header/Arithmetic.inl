#ifndef __LIBMATH__ARITHMETIC_INL__
#define __LIBMATH__ARITHMETIC_INL__

#include "Arithmetic.h"

namespace LibMath
{
	constexpr float floor(const float value)
	{
		const float intPart = static_cast<float>(static_cast<int>(value));

		if (intPart > value)
			return intPart - 1;

		return intPart;
	}

	constexpr float ceil(const float value)
	{
		const auto intPart = static_cast<float>(static_cast<int>(value));

		if (intPart < value)
			return intPart + 1;

		return intPart;
	}

	constexpr float round(const float value)
	{
		const auto intPart = static_cast<float>(static_cast<int>(value));
		const float decimalPart = value - intPart;

		if (decimalPart < .5f)
			return intPart;

		return intPart + 1;
	}

	template <typename T>
	constexpr T	clamp(T value, T a, T b)
	{
		const float minVal = min(a, b);
		const float maxVal = max(a, b);
		return max(minVal, min(value, maxVal));
	}

	template <typename T>
	constexpr T	snap(T value, T a, T b)
	{
		return abs(value - a) < abs(value - b) ? a : b;
	}

	constexpr float wrap(const float value, const float a, const float b)
	{
		const float min = a < b ? a : b;
		const float max = a > b ? a : b;

		return value - (max - min) * floor((value - min) / (max - min));
	}

	constexpr float squareRoot(const float value, float precision, const size_t maxSteps)
	{
		if (value < 0)
			return NAN;

		if (value == 0.f)
			return 0.f;

		if (precision == 0.f)
			precision = std::numeric_limits<float>::epsilon();
		else
			precision = abs(precision);

		float sqrt = value >= 4.f ? value / 2.f :
			value < .5f ? 1.f / (1 - value) :
			value < 1.f ? 1.f / value : value;

		// Keep repeating until the approximation is close enough to the real value
		for (size_t i = 0; (maxSteps == 0 || i < maxSteps) && abs(sqrt * sqrt - value) > precision; i++)
			sqrt = .5f * (sqrt + value / sqrt); // Babylonian step - cf. https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Heron's_method

		return sqrt;
	}

	constexpr float pow(const float value, const int exponent)
	{
		if (floatEquals(value, 1.f) || exponent == 0)
			return 1.f;

		if (floatEquals(value, 0))
			return 0.f;

		float result;

		if (exponent > 0)
		{
			result = value;

			for (int i = 1; i < exponent; i++)
				result *= value;
		}
		else
		{
			result = 1.f;

			for (int i = 0; i > exponent; i--)
				result /= value;
		}

		return result;
	}

	template <typename T>
	constexpr T	min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	constexpr T	max(T a, T b)
	{
		return a > b ? a : b;
	}

	constexpr float abs(const float value)
	{
		return value < 0 ? -value : value;
	}

	constexpr float sign(const float value)
	{
		return value < 0 ? -1.f : 1.f;
	}

	// adapted from https://stackoverflow.com/a/15012792
	constexpr bool floatEquals(const float a, const float b)
	{
		const float maxXYOne = max(max(1.0f, abs(a)), abs(b));

		return abs(a - b) <= std::numeric_limits<float>::epsilon() * maxXYOne;
	}

	template <typename T>
	constexpr bool isInRange(T value, T a, T b)
	{
		const float minVal = min(a, b);
		const float maxVal = max(a, b);

		return minVal <= value && value <= maxVal;
	}
}

#endif // !__LIBMATH__ARITHMETIC_INL__
