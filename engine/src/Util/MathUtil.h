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
 		
 		float smoothstep(float a, float b, float step);

 		float lerp(float a, float b, float amt);

 		bool IntersectTri(const glm::vec3 & _orig, const glm::vec3 & _dir, const glm::vec3 & _vert0, const glm::vec3 & _vert1, 
    					  const glm::vec3 & _vert2, float &u, float &v, float &dist);
	}
}