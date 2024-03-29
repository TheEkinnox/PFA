#include <cmath>

#include "Trigonometry.h"

namespace LibMath
{
	float sin(const Radian& angle)
	{
		return sinf(angle.radian());
	}

	float cos(const Radian& angle)
	{
		return cosf(angle.radian());
	}

	float tan(const Radian& angle)
	{
		return tanf(angle.radian());
	}

	Radian asin(const float val)
	{
		return Radian(asinf(val));
	}

	Radian acos(const float val)
	{
		return Radian(acosf(val));
	}

	Radian atan(const float val)
	{
		return Radian(atanf(val));
	}

	Radian atan(const float y, const float x)
	{
		return Radian(atan2f(y, x));
	}
}
