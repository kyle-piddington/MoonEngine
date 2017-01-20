#pragma once
#include "GL/GLVertexArrayObject.h"
/**
 * MeshInfo holds on to the information needed
 * to render a mesh that's already bound to the GPU.
 */
namespace MoonEngine
{
	struct MeshInfo
	{
		unsigned numVerts;
		GLsizei numTris;
		GLvoid * indexDataOffset;
		GLuint baseVertex;
		//Pointer to the vertex array object that contains the mesh.
		const GLVertexArrayObject * vertexObjectPtr;
	};	
}
