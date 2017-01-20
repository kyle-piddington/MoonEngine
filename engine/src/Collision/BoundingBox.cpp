#include "BoundingBox.h"
using namespace MoonEngine;
BoundingBox::BoundingBox():
minX(0),
maxX(0),
minY(0),
maxY(0),
minZ(0),
maxZ(0)
{

}

BoundingBox BoundingBox::BoundPoints(const std::vector<glm::vec3> & points)
{
	BoundingBox newBox;
	for(const glm::vec3 & point : points)
	{
		newBox.minX = std::min(newBox.minX, point.x);
		newBox.minY = std::min(newBox.minY, point.y);
		newBox.minZ = std::min(newBox.minZ, point.z);
		newBox.maxX = std::max(newBox.maxX, point.x);
		newBox.maxX = std::max(newBox.maxX, point.x);
		newBox.maxX = std::max(newBox.maxX, point.x);
			
	}
	return newBox;
}