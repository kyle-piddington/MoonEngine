#pragma once
#include "Component/Component.h"
#include "Collision/BoundingBox.h"
/**
 * A BoxCollider is an axis-aligned bounding box for a game object.
 * BoxColliders support AABB collision.
 */
namespace MoonEngine
{
	class BoxCollider : public Component
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
		 * @param  other another box collider
		 * @return       true if collides
		 */
		bool intersects(const BoxCollider * other);

		/**
		 * Return the current AABB
		 * @return [description]
		 */
		const BoundingBox & getBoundingBox();

		glm::vec3 getCenter();

	private:

		void createTransformedBox();

		bool _givenBox;
		BoundingBox _originalBox;
		BoundingBox _transformedBox;
	};	
}
