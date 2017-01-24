#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;
TEST_CASE("Test BBox Transformation", "[Bounding Box]")
{
	Transform t;
	BoundingBox box(-1,1,-1,1,-1,1);
	t.translate(glm::vec3(1,1,1));
	BoundingBox newBox = box.transform(t.getMatrix());
	REQUIRE(newBox.minX == 0);
	REQUIRE(newBox.maxX == 2);
	REQUIRE(newBox.minY == 0);
	REQUIRE(newBox.maxY == 2);
	REQUIRE(newBox.minZ == 0);
	REQUIRE(newBox.maxZ == 2);


}