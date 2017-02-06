#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;
TEST_CASE("Quadtree Basic Setup", "[Quadtree]")
{
	CDLODQuadtree tree;
	ImplicitHeightmapSource heightSource(256,256,[](int, int){return 0;});
	CDLODQuadtree::CreateInfo desc;
	desc.source = &heightSource;
	desc.leafNodeSize = 16;
	desc.LODLevelCount = 4;
	MapDimensions dims;
	dims.minCoords = glm::vec3(0,0,0);
	dims.size = glm::vec3(100,100,100);
	desc.dimensions = dims;
	tree.Create(desc);
}
TEST_CASE("Quadtree Basic Selection", "[Quadtree]")
{
	CDLODQuadtree tree;
	ImplicitHeightmapSource heightSource(256,256,[](int, int){return 0;});
	CDLODQuadtree::CreateInfo desc;
	desc.source = &heightSource;
	desc.leafNodeSize = 16;
	desc.LODLevelCount = 4;
	MapDimensions dims;
	dims.minCoords = glm::vec3(0,0,0);
	dims.size = glm::vec3(100,100,100);
	desc.dimensions = dims;
	tree.Create(desc);
	glm::vec4 frustrum [] = {
		glm::vec4(-1,0,0,20),
		glm::vec4(1,0,0,20),
		glm::vec4(0,-1,0,20),
		glm::vec4(0,1,0,20),
		glm::vec4(0,0,-1, 20),
		glm::vec4(0,0,1,20)
	};
	CDLODQuadtree::LODSelectionOnStack<1024> selectionInfo(
		glm::vec3(0,0,-5),
		1000.f,
		frustrum, 
		1.0f
		);
	tree.LODSelect(&selectionInfo);
	assert(selectionInfo.getSelectionCount() > 0);

}