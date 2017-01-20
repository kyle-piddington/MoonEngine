#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;
TEST_CASE("tinyOBJLoadMesh","[ObjectLoading]")
{
	GLBuffer vBuffer(GL_ARRAY_BUFFER),iBuffer(GL_ELEMENT_ARRAY_BUFFER);
	GLVertexArrayObject vao;
	MeshInfo info;
	REQUIRE(true == BasicLoader::LoadIntoBuffer("resources/bunny.obj", &vBuffer, &iBuffer, &vao, &info));
	REQUIRE(info.numVerts > 0);
	REQUIRE(info.numTris > 0);
	REQUIRE(info.indexDataOffset == nullptr);
	REQUIRE(info.baseVertex == 0);
	REQUIRE(info.vertexObjectPtr == &vao);

}