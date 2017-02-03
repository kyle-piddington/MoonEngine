#pragma once

#include "Component/Component.h"
#include "Collision/BoundingBox.h"
#include "Geometry/Hit.h"
/**
 * A BoxCollider is an axis-aligned bounding box for a game object.
 * BoxColliders support AABB collision.
 */
namespace MoonEngine
{
    class BoxCollider: public Component
    {
    public:
        /**
         * Create a boxCollider. The component will
         * check the gameObject to determine if it can read
         * and create an AABB from the mesh.
         */
        BoxCollider();

        /**
         * Create a boxCollider, and specify a maximum and minimum coordinate
         * set.
         */
        BoxCollider(glm::vec3 minCoords, glm::vec3 maxCoords);

        /**
         * Initialize the AABB
         */
        void start();

        /**
         * Update transformed AABB
         */
        void update(float dt);

        /**
         * Determine if one boxCollider intersects another
         * @param1  other another box collider
         * @param2  pointer to vec3 that will contain the collision normal
         * @return       true if collides
         */
        bool intersects(const BoxCollider * other, glm::vec3 * colnormal);

        /**
         * Intersect the box with a ray
         * @param  origin    the origin of the ray
         * @param  direction direction of the ray
         * @param  hit       Info returned after
         * @return           true if interesection
         */
        bool intersectsRay(glm::vec3 origin, glm::vec3 direction, Hit * hit);


        /**
         * Return the current AABB
         * @return [description]
         */
        const BoundingBox & getBoundingBox();

        glm::vec3 getCenter();

        virtual std::shared_ptr<Component> clone() const;

        glm::vec3 getHalfWidths();

    private:

        void createTransformedBox();

        bool _givenBox;
        BoundingBox _originalBox;
        BoundingBox _transformedBox;
    };
}
