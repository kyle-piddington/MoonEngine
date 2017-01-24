#include "catch.hpp"
#include "MoonEngine.h"
#include <iostream>
using namespace MoonEngine;
TEST_CASE("Test BBox Transformation", "[Bounding Box]")
{
	Transform t;
	BoundingBox box(-1,1,-1,1,-1,1);
	REQUIRE(glm::length(box.centerPoint - glm::vec3(0,0,0)) < 1e-3);
	REQUIRE(box.xHalfWidth == 1.0f);
	REQUIRE(box.yHalfWidth == 1.0f);
	REQUIRE(box.zHalfWidth == 1.0f);
	
	t.translate(glm::vec3(1,1,1));
	BoundingBox newBox = box.transform(t.getMatrix());
	
	REQUIRE(glm::length(newBox.centerPoint - glm::vec3(1,1,1)) < 1e-3);


}