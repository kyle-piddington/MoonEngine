#pragma once

#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

namespace MoonEngine
{
    namespace MathUtil
    {

        int sign(float x);

        float clamp(float x, float min, float max);

        /**
         * Interpolate towards a target
         * @param  current  current value
         * @param  target   target value
         * @param  maxDelta how far will the target move each frame?
         * @return          an interpolated value
         */
        float moveTowards(float current, float target, float maxDelta);

        glm::vec3 moveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta);

    }
}