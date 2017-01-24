#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace MoonEngine
{
	class BoundingBox
	{
	public:
		BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
		BoundingBox();
		static BoundingBox BoundPoints(const std::vector<glm::vec3> & points);
		BoundingBox transform(const glm::mat4 & transformation);
		bool intersects(const BoundingBox & other);
		float minX;
		float maxX;
		float minY;
		float maxY;
		float minZ;
		float maxZ;

		std::vector<glm::vec3> cornerPoints();
	};
}
