#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "IntersectionType.h"
#include <algorithm>
#include <string>
namespace MoonEngine
{

    class BoundingBox
    {
    public:

        BoundingBox(glm::vec3 center, float hWidthX, float hWidthY, float hWidthZ);

        BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

        BoundingBox();

        static BoundingBox BoundPoints(const std::vector<glm::vec3> & points);

        BoundingBox transform(const glm::mat4 & transformation) const;

        bool intersects(const BoundingBox & other, glm::vec3 * colnormal);

        /**
         * Ray-line intersection
         * @param  origin    origin of the ray
         * @param  direction direction of the ray
         * @param  colnormal output of the normal
         * @return           the distance along ray, or -1
         */
        float intersectsRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 * colnormal);



        std::vector<glm::vec3> cornerPoints() const;

        bool inFrustrum(const glm::vec4 frustrum[6]) const;

        IntersectType testFrustrum(const glm::vec4 frustrum[6]) const;


        float minDistFromPointSq( const glm::vec3 & point )
        {
          float dist = 0.0f;
          float k = 0;

          k = std::min( fabsf( point.x - (centerPoint.x - xHalfWidth)),  fabsf( point.x - (centerPoint.x + xHalfWidth ) ));
          dist += k * k;

          k = std::min( fabsf( point.y - (centerPoint.y - yHalfWidth) ), fabsf( point.y - (centerPoint.y + yHalfWidth ) ));
          dist += k * k;

          k = std::min( fabsf( point.z - (centerPoint.z - zHalfWidth) ), fabsf( point.z - (centerPoint.z + zHalfWidth ) ));
          dist += k * k;

          return dist;
        }
        float maxDistFromPointSq( const glm::vec3 & point )
        {
          float dist = 0.0f;
          float k;

          k = std::max( fabsf( point.x - (centerPoint.x - xHalfWidth )), fabsf( point.x - (centerPoint.x + xHalfWidth) ) );
          dist += k * k;

          k = std::max( fabsf( point.y - (centerPoint.y - yHalfWidth )), fabsf( point.y - (centerPoint.y + yHalfWidth) ) );
          dist += k * k;

          k = std::max( fabsf( point.z - (centerPoint.z - zHalfWidth )), fabsf( point.z - (centerPoint.z + zHalfWidth) ) );
          dist += k * k;

          return dist;
        }

      
        bool intersectSphereSq(glm::vec3 sphereCenter, float rSq)
        {
            return minDistFromPointSq(sphereCenter) <= rSq;
        }
        bool insideSphereSq(glm::vec3 sphereCenter, float rSq)
        {
            return maxDistFromPointSq(sphereCenter) <= rSq;
        }

        bool contains(const BoundingBox & other) const;
        glm::vec3 max() const;
        glm::vec3 min() const;

        glm::vec3 centerPoint;
        float xHalfWidth;
        float yHalfWidth;
        float zHalfWidth;

        std::string toString() const
        {
          glm::vec3 min = this->min();
          glm::vec3 max = this->max();
          return "[" + 
          std::to_string(min.x ) + "," +
          std::to_string(min.y ) + "," +
          std::to_string(min.z ) + "," + 
          std::to_string(max.x ) + "," + 
          std::to_string(max.y ) + "," + 
          std::to_string(max.z ) + "]";

        }
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
