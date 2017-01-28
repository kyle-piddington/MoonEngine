#pragma once
#include <cmath>
#include <algorithm>
namespace MoonEngine
{
	namespace MathUtil{

		int sign(float x)
		{
			if (x > 0) return 1;
			if (x < 0) return -1;
			return 0;
		}
		float clamp(float x, float min, float max)
		{
			return std::max(std::min(x,max),min);
		}
		/**
		 * Interpolate towards a target
		 * @param  current  current value
		 * @param  target   target value
		 * @param  maxDelta how far will the target move each frame?
		 * @return          an interpolated value
		 */
		float moveTowards(float current, float target, float maxDelta)
		{
			if (std::abs(target - current) <= maxDelta)
			{
				return target;
			}
			return current + sign(target - current) * maxDelta;
		}
	}
}