#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;

GLBuffer genBuffer(GLuint * capValue)
{

	GLBuffer bufA =  GLBuffer(GL_ARRAY_BUFFER);
	*capValue = bufA.getObject();
	return bufA;
}
TEST_CASE("GL Buffer sanity tests", "[GLBuffer]")
{
	//Test getting a buffer
	GLBuffer bufferA = GLBuffer(GL_ARRAY_BUFFER);
	REQUIRE(bufferA.getObject() != 0);
	GLuint bufferObject;
	GLBuffer bufferB = genBuffer(&bufferObject);
	REQUIRE(bufferB.getObject() == bufferObject);
	REQUIRE(bufferB.getType() == GL_ARRAY_BUFFER);

}