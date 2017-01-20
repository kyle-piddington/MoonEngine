/**
 * Load an OBJ model and read it into an interchange mesh format.
 */

#include "GL/GLBuffer.h"
#include "GL/GLVertexArrayObject.h"
#include "GL/GLConstants.h"
#include "MeshInfo.h"
#include <string>
namespace MoonEngine
{
	class BasicLoader
	{
	public:
		static bool LoadIntoBuffer(std::string fileName,
			 GLBuffer * vertexBuffer,
			 GLBuffer * indexBuffer,
			 GLVertexArrayObject * vao,
			 MeshInfo * outInfo);
	};
};