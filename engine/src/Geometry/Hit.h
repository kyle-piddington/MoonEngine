#pragma once

/**
 * Output of a Raycast
 */
namespace MoonEngine
{
    struct Hit
    {
        GameObject * object;
        glm::vec3 intersectionPoint;
        glm::vec3 intersectionNormal;
        float distance;
    };
}

