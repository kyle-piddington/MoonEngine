#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace MoonEngine
{
    class BoundingBox
    {
    public:
        BoundingBox(glm::vec3 center, float hWidthX, float hWidthY, float hWidthZ);

        BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

        BoundingBox();

        static BoundingBox BoundPoints(const std::vector<glm::vec3> & points);

        BoundingBox transform(const glm::mat4 & transformation);

        bool intersects(const BoundingBox & other, glm::vec3 * colnormal);

        /**
         * Ray-line intersection
         * @param  origin    origin of the ray
         * @param  direction direction of the ray
         * @param  colnormal output of the normal
         * @return           the distance along ray, or -1
         */
        float intersectsRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 * colnormal);

        glm::vec3 centerPoint;
        float xHalfWidth;
        float yHalfWidth;
        float zHalfWidth;

        std::vector<glm::vec3> cornerPoints();

    private:
        enum Plane
        {
            X_NEAR,
            X_FAR,
            Y_NEAR,
            Y_FAR,
            Z_NEAR,
            Z_FAR
        };

        glm::vec3 normalFor(BoundingBox::Plane plane);

    };
}
