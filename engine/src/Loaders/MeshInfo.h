#pragma once
#include "GL/GLVertexArrayObject.h"
#include "Collision/BoundingBox.h"
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
		//Bounding min and max vertices
		BoundingBox boundingBox;
		void bind() const{vertexObjectPtr->bind();}
		void setVertexArrayObject(GLVertexArrayObject * vao)
		{
			vertexObjectPtr = vao;
		}
		private:
		//Pointer to the vertex array object that contains the mesh.
		////Most meshes are saved  in MeshLibrary, but not all.
		const GLVertexArrayObject * vertexObjectPtr;
		
	};	
}
