#pragma once
/**
World contains game constants for world coordinate systems
*/
#include <glm/glm.hpp>

namespace MoonEngine
{
    struct World
    {
        static glm::vec3 Up;
        static glm::vec3 Right;
        static glm::vec3 Forward;

    };
}
