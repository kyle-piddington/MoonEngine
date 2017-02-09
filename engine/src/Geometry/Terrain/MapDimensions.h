#pragma once
/**
 * Describes a quadtree map.
 * Taken from CDLOD
 */
#include <glm/glm.hpp>
namespace MoonEngine
{
	struct MapDimensions
	{
		glm::vec3 minCoords;
		glm::vec3 size;

		float                MaxX() const   { return minCoords.x + size.x; }
		float                MaxY() const   { return minCoords.y + size.y; }
		float                MaxZ() const   { return minCoords.z + size.z; }
	};

}
