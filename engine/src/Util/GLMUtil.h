#pragma once

#include <glm/glm.hpp>
#include "assimp/matrix4x4.h"
namespace MoonEngine
{
    namespace GLMUtil
    {
        /**
         * Convert little-endian hex code to vector3
         * useful for color codes
         * @param  hexCode color code
         * @return         a floating point vector for color
         */
        glm::vec3 FromHex(unsigned hexCode);

        glm::mat4 FromAssimp(const aiMatrix4x4 & mat);

        
    }
}