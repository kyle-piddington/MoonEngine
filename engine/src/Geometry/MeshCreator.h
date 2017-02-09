#pragma once

#include "Loaders/MeshInfo.h"
#include <glm/glm.hpp>
#include <memory>
#include "GLWrapper/GLVertexArrayObject.h"
#include "GLWrapper/GLBuffer.h"

namespace MoonEngine
{

	struct GridInfo
	{
		int tlIdx;
		int trIdx;
		int blIdx;
		int brIdx;
		int gridSize;
		MeshInfo * meshInfo;
	};

	class MeshCreator
	{
	public:
		static MeshInfo * CreateQuad(glm::vec2 lowerLeft, glm::vec2 upperRight, int divX = 2, int divY = 2);
		static GridInfo   CreateGrid(glm::vec2 lowerLeft, glm::vec2 upperRight, int vertDim);
	private:
		static std::vector<std::shared_ptr<GLBuffer>> backingBuffers;
		static std::vector<std::shared_ptr<GLVertexArrayObject>> backingArrayObjects;
	
	};
}