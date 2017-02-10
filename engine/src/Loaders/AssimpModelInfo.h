#pragma once
#include "GLWrapper/GLVertexArrayObject.h"
#include "Collision/BoundingBox.h"
#include "MeshInfo.h"
#include <string>
#include <unordered_map>
//AssimpInfo contains all
//data imported by assimp, metadata
//about the mesh, and a meshInfo 
//for binding later.
namespace MoonEngine
{
	struct AssimpMeshInfo
	{
		std::unordered_map<std::string, std::string> textures;
		MeshInfo meshInfo;
		BoundingBox box;
	};

	/**
	 * Contains all information about a model loaded by Assimp
	 * (+ Textures, full mesh info, bone and armature data (Eventually!))
	 */
	struct AssimpModelInfo
	{

        //Bounding min and max vertices
		BoundingBox boundingBox;

		bool hasNormals() const;

		bool hasTangentBitangent() const;

		bool hasBones() const;

		std::vector<MeshInfo> getMeshInfo() const;

		MeshInfo getFullMeshInfo() const;

		std::vector<std::string> getAllTextures() const;

		void setVertexArrayObject(GLVertexArrayObject * vao)
		{
			vertexObjectPtr = vao;
		}
		//VAO contains full mesh information.
		inline GLVertexArrayObject * getVAO()
		{
			return this->vertexObjectPtr;
		}
		//Mesh info avaliable if no other
		//transformations / information is needed.
		//(Useful for basic mesh display)
		
	private:
        //Pointer to the vertex array object that contains the mesh.
        //Most meshes are saved  in MeshLibrary, but not all.
		std::vector<MeshInfo> _meshInfo;
		MeshInfo fullMeshInfo;
		GLVertexArrayObject * vertexObjectPtr;

		int components

	};
	
}
