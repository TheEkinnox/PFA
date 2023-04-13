#ifndef __LIBMATH__INTERPOLATION_H__
#define __LIBMATH__INTERPOLATION_H__

namespace LibMath
{
	template <typename ValT, typename ProgressT>
	constexpr ValT lerp(const ValT a, const ValT b, const ProgressT t)
	{
		return a + (b - a) * t;
	}
}

#endif // !__LIBMATH__INTERSECTION_H__
