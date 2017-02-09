#include "BoundingBox.h"
#include <algorithm>
#include "Util/Logger.h"
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
    yHalfWidth = (maxY - minY) / 2;
    zHalfWidth = (maxZ - minZ) / 2;

    centerPoint = glm::vec3(minX + xHalfWidth, minY + yHalfWidth, minZ + zHalfWidth);
}

BoundingBox::BoundingBox():
    centerPoint(0),
    xHalfWidth(0),
    yHalfWidth(0),
    zHalfWidth(0)
{

}

float BoundingBox::intersectsRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 * colnormal)
{
    BoundingBox::Plane minPlane = BoundingBox::Plane::X_NEAR;
    BoundingBox::Plane maxPlane = BoundingBox::Plane::X_FAR;

    float tmin = -INT_MAX;
    float tmax = INT_MAX;
    glm::vec3 minCoords = min();//centerPoint + glm::vec3(-xHalfWidth, -yHalfWidth, -zHalfWidth);
    glm::vec3 maxCoords = max();//centerPoint + glm::vec3(xHalfWidth, yHalfWidth, zHalfWidth);


    for (int i = 0; i < 3; ++i)
    {


        if (fabsf(direction[i]) > 0)
        {
            float t1 = (minCoords[i] - origin[i]) / direction[i];
            float t2 = (maxCoords[i] - origin[i]) / direction[i];
            bool swp = false;
            if (t1 > t2)
            {
                float tmp = t2;
                t2 = t1;
                t1 = tmp;
                swp = true;
            }
            if (t1 > tmin)
            {
                minPlane = (BoundingBox::Plane) (2 * i + swp);
            }
            if (t2 < tmax)
            {
                maxPlane = (BoundingBox::Plane) (2 * i + 1 - swp);
            }
            tmin = fmaxf(tmin, t1);
            tmax = fminf(tmax, t2);

        }
        else if (origin[i] < minCoords[i] || origin[i] > maxCoords[i])
        {
            return -1;
        }
    }
 
    
    //Tmax must be greater than zero, and tMin.
    if (tmax >= fmaxf(tmin, 0.0))
    {
        //Return lowest non-negative
        BoundingBox::Plane intersectPlane = minPlane;
        float t = tmin;
        if (t < 0)
        {
            //Internal intersection
            t = tmax;
            intersectPlane = maxPlane;
        }
        //Create normal by determining which plane the point is on
        if (colnormal != nullptr)
        {
            *colnormal = normalFor(intersectPlane);
        }

        return t;
    }
    else
    {
        return -1;
    }
}


BoundingBox BoundingBox::BoundPoints(const std::vector<glm::vec3> & points)
{
    float minX, minY, minZ, maxX, maxY, maxZ;
    assert(points.size() > 0);
    minX = maxX = points[0].x;
    minY = maxY = points[0].y;
    minZ = maxZ = points[0].z;

    for (const glm::vec3 & point : points)
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

bool BoundingBox::contains(const glm::vec3 & point) const
{
    bool xInRange = fabsf(point.x - centerPoint.x) < xHalfWidth;
    bool yInRange = fabsf(point.y - centerPoint.y) < yHalfWidth;
    bool zInRange = fabsf(point.z - centerPoint.z) < zHalfWidth;
    return xInRange && yInRange && zInRange;      
}

BoundingBox BoundingBox::transform(const glm::mat4 & transformation) const
{
    std::vector<glm::vec3> boundingPoints = cornerPoints();
    for (glm::vec3 & point : boundingPoints)
    {
        point = glm::vec3(
                transformation * glm::vec4(point.x, point.y, point.z, 1));
    }
    return BoundingBox::BoundPoints(boundingPoints);
}

glm::vec3 BoundingBox::max() const
{
	return glm::vec3(centerPoint.x + xHalfWidth, centerPoint.y + yHalfWidth, centerPoint.z + zHalfWidth);
}

glm::vec3 BoundingBox::min() const
{
	return glm::vec3(centerPoint.x - xHalfWidth, centerPoint.y - yHalfWidth, centerPoint.z - zHalfWidth);
}

std::vector<glm::vec3> BoundingBox::cornerPoints() const
{
    std::vector<glm::vec3> boundPoints;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                boundPoints.push_back(centerPoint + glm::vec3(
                        i == 0 ? -xHalfWidth : xHalfWidth,
                        j == 0 ? -yHalfWidth : yHalfWidth,
                        k == 0 ? -zHalfWidth : zHalfWidth));
            }
        }
    }
    return boundPoints;
}

//https://studiofreya.com/3d-math-and-physics/simple-aabb-vs-aabb-collision-detection/
bool BoundingBox::intersects(const BoundingBox & other, glm::vec3 * colnormal)
{
    glm::vec3 nor;
    bool x = std::abs(centerPoint.x - other.centerPoint.x) <= (xHalfWidth + other.xHalfWidth);
    if (x && centerPoint.x < other.centerPoint.x)
        nor = glm::vec3(-1.0f, 0.0f, 0.0f);
    else if (x)
        nor = glm::vec3(1.0f, 0.0f, 0.0f);

    bool y = std::abs(centerPoint.y - other.centerPoint.y) <= (yHalfWidth + other.yHalfWidth);
    if (y && centerPoint.y < other.centerPoint.y)
        nor = glm::vec3(0.0f, -1.0f, 0.0f);
    else if (y)
        nor = glm::vec3(0.0f, 1.0f, 0.0f);

    bool z = std::abs(centerPoint.z - other.centerPoint.z) <= (zHalfWidth + other.zHalfWidth);
    if (z && centerPoint.z < other.centerPoint.z)
        nor = glm::vec3(0.0f, 0.0f, -1.0f);
    else if (z)
        nor = glm::vec3(0.0f, 0.0f, 1.0f);

    //If an interesction occurs, calculate
    //the normal via a raycast
    if (x && y && z && colnormal != nullptr)
    {
        intersectsRay(other.centerPoint,
                      glm::normalize(centerPoint - other.centerPoint), colnormal);
    }

    return x && y && z;
}


bool BoundingBox::inFrustrum(const glm::vec4 frustrum[6]) const
{
    glm::vec3 box[2];
    box[0] = min();
    box[1] = max();
    float distance;
    

    for (int j = 0; j < 6; j++)
    {
        int ix = static_cast<int>(frustrum[j].x > 0.0f);
        int iy = static_cast<int>(frustrum[j].y > 0.0f);
        int iz = static_cast<int>(frustrum[j].z > 0.0f);

        distance = (frustrum[j].x * box[ix].x + 
            frustrum[j].y * box[iy].y + 
            frustrum[j].z * box[iz].z);
        if (distance < -frustrum[j].w)
        {
            return false;
            break;
        }
    }
    return true;
}
IntersectType BoundingBox::testFrustrum(const glm::vec4 frustrum[6]) const
{
    if(!inFrustrum(frustrum))
    {
        return IT_Outside;
    }
    else
    {
        
        float distance;
        auto pts = cornerPoints();
        for(int i = 0; i < pts.size(); i++)
        {
            for(int j = 0; j < 6; j++)
            {
                distance = (
                    frustrum[j].x * pts[i].x + 
                    frustrum[j].y * pts[i].y + 
                    frustrum[j].z * pts[i].z);
                if (distance < -frustrum[j].w)
                {
                    return IT_Intersect;
                    

                }
            }
        }
 
        return IT_Inside;     
    }
}

glm::vec3 BoundingBox::normalFor(BoundingBox::Plane plane){
    switch (plane)
    {
        case X_NEAR:
            return glm::vec3(-1, 0, 0);
        case X_FAR:
            return glm::vec3(1, 0, 0);
        case Y_NEAR:
            return glm::vec3(0, -1, 0);
        case Y_FAR:
            return glm::vec3(0, 1, 0);
        case Z_NEAR:
            return glm::vec3(0, 0, -1);
        case Z_FAR:
            return glm::vec3(0, 0, 1);
    }
}
