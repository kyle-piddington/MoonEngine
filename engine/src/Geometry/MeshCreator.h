#ifndef __MeshCreator_H__
#define __MeshCreator_H__ 
#include "Loaders/MeshInfo.h"
#include <glm/glm.hpp>
#include <memory>
#include "GLWrapper/GLVertexArrayObject.h"
#include "GLWrapper/GLBuffer.h"
namespace MoonEngine
{
	class MeshCreator
	{
	public:
		static MeshInfo * CreateQuad(glm::vec2 lowerLeft, glm::vec2 upperRight);
	private:
		static std::vector<std::shared_ptr<GLBuffer>> backingBuffers;
		static std::vector<std::shared_ptr<GLVertexArrayObject>> backingArrayObjects;
	};
}

#endif