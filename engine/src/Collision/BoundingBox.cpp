#include "BoundingBox.h"
using namespace MoonEngine;

BoundingBox::BoundingBox(
	float minX,
	float maxX,
	float minY,
	float maxY,
	float minZ,
	float maxZ):
minX(minX),
maxX(maxX),
minY(minY),
maxY(maxY),
minZ(minZ),
maxZ(maxZ)
{

}

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
		newBox.maxY = std::max(newBox.maxY, point.y);
		newBox.maxZ = std::max(newBox.maxZ, point.z);
			
	}
	return newBox;
}

BoundingBox BoundingBox::transform(const glm::mat4 & transformation)
{
	std::vector<glm::vec3> boundingPoints = cornerPoints();
	std::vector<glm::vec3> returnedPoints;
	returnedPoints.reserve(boundingPoints.size());
	for(glm::vec3 & point : boundingPoints)
	{
		returnedPoints.push_back(glm::vec3(
			transformation * glm::vec4(point.x,point.y,point.z,1)));
	}
	return BoundingBox::BoundPoints(returnedPoints);
}

std::vector<glm::vec3> BoundingBox::cornerPoints()
{
	std::vector<glm::vec3> boundPoints;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int k = 0; k < 2; k++)
			{
				boundPoints.push_back(glm::vec3(
					i ? minX : maxX, 
					j ? minY : maxY,
					k ? minZ : maxZ));
			}
		}
	}
	return boundPoints;
}