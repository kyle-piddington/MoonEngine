#include "MoonEngine.h"
#include "catch.hpp"
using namespace MoonEngine;
TEST_CASE("Basic getComponent test", "[Camera component]")
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	Camera cam(M_PI/3, 1,0.5,50);
	obj->addComponent(&cam);
	cam.start();
	REQUIRE(obj->getComponent<Camera>() == &cam);
	
}