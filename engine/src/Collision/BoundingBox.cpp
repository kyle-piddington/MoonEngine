#include "BoundingBox.h"
using namespace MoonEngine;


BoundingBox::BoundingBox(glm::vec3 center, float xHalfWidth, float yHalfWidth, float zHalfWidth):
centerPoint(center),
xHalfWidth(xHalfWidth),
yHalfWidth(yHalfWidth),
zHalfWidth(zHalfWidth)
{
}
BoundingBox::BoundingBox(
	float minX,
	float maxX,
	float minY,
	float maxY,
	float minZ,
	float maxZ)
{
	assert(minX <= maxX);
	assert(minY <= maxY);
	assert(minZ <= maxZ);
	
	xHalfWidth = (maxX - minX) / 2;
	yHalfWidth = (maxY - minY)/ 2;
	zHalfWidth = (maxZ -minZ) / 2;
	
	centerPoint = glm::vec3(minX + xHalfWidth,
							minY + yHalfWidth,
							minZ + zHalfWidth);
	
}

BoundingBox::BoundingBox():
centerPoint(0),
xHalfWidth(0),
yHalfWidth(0),
zHalfWidth(0)
{

}

BoundingBox BoundingBox::BoundPoints(const std::vector<glm::vec3> & points)
{
	float minX, minY, minZ, maxX, maxY, maxZ;
	assert(points.size() > 0);
	minX = maxX = points[0].x;
	minY = maxY = points[0].y;
	minZ = maxZ = points[0].z;
	
	for(const glm::vec3 & point : points)
	{
		minX = std::min(minX, point.x);
		minY = std::min(minY, point.y);
		minZ = std::min(minZ, point.z);
		maxX = std::max(maxX, point.x);
		maxY = std::max(maxY, point.y);
		maxZ = std::max(maxZ, point.z);
			
	}

	return BoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
}

BoundingBox BoundingBox::transform(const glm::mat4 & transformation)
{
	std::vector<glm::vec3> boundingPoints = cornerPoints();
	for(glm::vec3 & point : boundingPoints)
	{
		point = glm::vec3(
			transformation * glm::vec4(point.x,point.y,point.z,1));
	}
	return BoundingBox::BoundPoints(boundingPoints);
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
				boundPoints.push_back(centerPoint  + glm::vec3(
					i == 0 ? -xHalfWidth :  xHalfWidth, 
					j == 0 ? -yHalfWidth :  yHalfWidth,
					k == 0 ? -zHalfWidth :  zHalfWidth));
			}
		}
	}
	return boundPoints;
}

//https://studiofreya.com/3d-math-and-physics/simple-aabb-vs-aabb-collision-detection/
bool BoundingBox::intersects(const BoundingBox & other)
{
	
	bool x = std::abs(centerPoint.x - other.centerPoint.x) <= (xHalfWidth + other.xHalfWidth);
    bool y = std::abs(centerPoint.y - other.centerPoint.y) <= (yHalfWidth + other.yHalfWidth);
    bool z = std::abs(centerPoint.z - other.centerPoint.z) <= (zHalfWidth + other.zHalfWidth);
    return x && y && z;
}