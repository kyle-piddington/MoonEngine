#include "catch.hpp"
#include "MoonEngine.h"
#include <iostream>
using namespace MoonEngine;

TEST_CASE("Glm from hex", "[GLM Util]")
{
	
	glm::vec3 color = GLMUtil::FromHex(0xFF80FF);
	REQUIRE(color.r == 1.0f);
	REQUIRE(std::abs(color.g - 0.5) < 1e-2);
	REQUIRE(color.r == 1.0f);

}