#pragma once
#include "GLWrapper/GLVertexArrayObject.h"
#include "Collision/BoundingBox.h"
#include "BasicMeshInfo.h"
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
		BasicMeshInfo meshInfo;
		BoundingBox box;

		void bind() const
		{
			meshInfo.bind();
		}
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

		bool hasTextureCoordinates() const;

		bool hasBones() const;

		const std::vector<AssimpMeshInfo> & getMeshInfo() const;

		int stride() const;

		//const MeshInfo & getFullMeshInfo() const;

		std::vector<std::string> getAllTextures() const;

		void addMeshInfo(AssimpMeshInfo & meshInfo);
		
		void setVertexArrayObject(GLVertexArrayObject * vao)
		{
			vertexObjectPtr = vao;
		}
		//VAO contains full mesh information.
		inline GLVertexArrayObject * getVAO()
		{
			return this->vertexObjectPtr;
		}

		void setHasNormals(bool normals)
		{
			_hasNormals = normals;
		}
		void setHasTangentBitangent(bool tbt)
		{
			_hasTangentBitangent = tbt;
		}
		void setHasTextureCoordinates(bool txCoord)
		{
			_hasTextureCoordinates  = txCoord;	
		}

		//Mesh info avaliable if no other
		//transformations / information is needed.
		//(Useful for basic mesh display)
		
	private:
        //Pointer to the vertex array object that contains the mesh.
        //Most meshes are saved  in MeshLibrary, but not all.
		std::vector<AssimpMeshInfo> _meshInfo;
		BasicMeshInfo fullMeshInfo;
		GLVertexArrayObject * vertexObjectPtr;
		bool _hasNormals;
		bool _hasTangentBitangent;
		bool _hasBones;
		bool _hasTextureCoordinates;
		

	};
	
}
