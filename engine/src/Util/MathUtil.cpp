#include "MathUtil.h"
#include <glm/gtc/type_ptr.hpp>
using namespace MoonEngine;

int MathUtil::sign(float x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

float MathUtil::clamp(float x, float min, float max)
{
    return std::max(std::min(x, max), min);
}

/**
 * Interpolate towards a target
 * @param  current  current value
 * @param  target   target value
 * @param  maxDelta how far will the target move each frame?
 * @return          an interpolated value
 */
float MathUtil::moveTowards(float current, float target, float maxDelta)
{
    if (std::abs(target - current) <= maxDelta)
    {
        return target;
    }
    return current + sign(target - current) * maxDelta;
}

glm::vec3 MathUtil::moveTowards(glm::vec3 current, glm::vec3 target, float maxDistanceDelta)
{	
	glm::vec3 a = target - current;
	float magnitude = glm::length(a);
	if (magnitude <= maxDistanceDelta || magnitude == 0.f)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;
}

float MathUtil::lerp(float a, float b, float amt)
{
	return a + (b - a) * amt;
}
//Provided from AMD
float MathUtil::smoothstep(float edge0, float edge1, float x)
{
    

    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

#define _IT_CROSS(dest,v1,v2) \
dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define _IT_DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define _IT_SUB(dest,v1,v2) \
dest[0]=v1[0]-v2[0]; \
dest[1]=v1[1]-v2[1]; \
dest[2]=v1[2]-v2[2]; 
bool MathUtil::IntersectTri(const glm::vec3 & _orig, const glm::vec3 & _dir, const glm::vec3 & _vert0, const glm::vec3 & _vert1, 
    const glm::vec3 & _vert2, float &u, float &v, float &dist)
{
    const float c_epsilon = 1e-3f;

    const float * orig =  glm::value_ptr(_orig);
    const float * dir =   glm::value_ptr(_dir);
    const float * vert0 = glm::value_ptr(_vert0);
    const float * vert1 = glm::value_ptr(_vert1);
    const float * vert2 = glm::value_ptr(_vert2);

    float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    float det,inv_det;

   // find vectors for two edges sharing vert0
    _IT_SUB(edge1, vert1, vert0);
    _IT_SUB(edge2, vert2, vert0);

   // begin calculating determinant - also used to calculate U parameter
    _IT_CROSS(pvec, dir, edge2);

   // if determinant is near zero, ray lies in plane of triangle 
    det = _IT_DOT(edge1, pvec);

   // calculate distance from vert0 to ray origin 
    _IT_SUB(tvec, orig, vert0);
    inv_det = 1.0f / det;

    if (det > c_epsilon)
    {
      // calculate U parameter and test bounds 
        u = _IT_DOT(tvec, pvec);
        if (u < 0.0 || u > det)
            return false;

      // prepare to test V parameter 
        _IT_CROSS(qvec, tvec, edge1);

      // calculate V parameter and test bounds 
        v = _IT_DOT(dir, qvec);
        if (v < 0.0 || u + v > det)
            return false;

    }
    else if(det < -c_epsilon)
    {
      // calculate U parameter and test bounds 
        u = _IT_DOT(tvec, pvec);
        if (u > 0.0 || u < det)
            return false;

      // prepare to test V parameter 
        _IT_CROSS(qvec, tvec, edge1);

      // calculate V parameter and test bounds 
        v = _IT_DOT(dir, qvec) ;
        if (v > 0.0 || u + v < det)
            return false;
    }
   else return false;  // ray is parallel to the plane of the triangle 

   // calculate t, ray intersects triangle 
   dist = _IT_DOT(edge2, qvec) * inv_det;
   u *= inv_det;
   v *= inv_det;
   
   return dist >= 0;
}

float MathUtil::toRadians(float degrees) {
    return degrees * (float)M_PI / 180.0f;
}
float MathUtil::toDegrees(float radians) {
    return radians * 180.0f / (float)M_PI;
}

