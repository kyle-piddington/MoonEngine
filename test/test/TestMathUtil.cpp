#include "MoonEngine.h"
#include "catch.hpp"
using namespace MoonEngine;
TEST_CASE("TriangleIntersect", "[Math]")
{
	glm::vec3 va (-0.5,-0.5,0);
	glm::vec3 vb (0.5,-0.5,0);
	glm::vec3 vc (0,0.5,0);
	float u;
	float v;
	float dist;
	REQUIRE(MathUtil::IntersectTri(glm::vec3(0,0,-3), glm::vec3(0,0,1), va, vb, vc, u, v, dist));
}