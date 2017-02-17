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


TEST_CASE("Test BBox Zero Height INtersect", "[Bounding Box]")
{
	Transform t;
	BoundingBox box(-1,1,-1,1,-1,1);
	REQUIRE(box.intersectsRay(glm::vec3(2,0,0), glm::vec3(-1,0,0), nullptr) > 0);
	REQUIRE(box.intersectsRay(glm::vec3(0,2,0), glm::vec3(0,-1,0), nullptr) > 0);
}
TEST_CASE ("Test BBox contains", "[Bounding box]")
{
	BoundingBox box1(-1,1,-1,1,-1,1);
	BoundingBox box2(-0.5f,0.5f,-0.5f,0.5f,-0.5f,0.5f);
	BoundingBox box3(-1 + 0.5,1 + 0.5,-1 + 0.5,1 + 0.5,-1 + 0.5,1 + 0.5);
	
	REQUIRE(box1.contains(box2));
	REQUIRE(!box2.contains(box1));
	REQUIRE(!box1.contains(box3));

}