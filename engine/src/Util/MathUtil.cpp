#include "MathUtil.h"

using namespace MoonEngine;

int MathUtil::sign(float x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

float MathUtil::clamp(float x, float min, float max)
{
    return std::max(std::min(x, max), min);
}

/**
 * Interpolate towards a target
 * @param  current  current value
 * @param  target   target value
 * @param  maxDelta how far will the target move each frame?
 * @return          an interpolated value
 */
float MathUtil::moveTowards(float current, float target, float maxDelta)
{
    if (std::abs(target - current) <= maxDelta)
    {
        return target;
    }
    return current + sign(target - current) * maxDelta;
}

glm::vec3 MathUtil::moveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta)

{	
	glm::vec3 a = target - current;
	float magnitude = glm::length(a);
	if (magnitude <= maxDistanceDelta || magnitude == 0.f)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;
}

float MathUtil::lerp(float a, float b, float amt)
{
	return a + (b - a) * amt;
}
