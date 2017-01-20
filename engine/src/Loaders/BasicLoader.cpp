#include "BasicLoader.h"
#include "thirdparty/tiny_obj_loader.h"
#include "Util/Logger.h"
#include "GL/GLConstants.h"

using namespace MoonEngine;
using namespace tinyobj;
bool BasicLoader::LoadIntoBuffer(std::string fileName,
	GLBuffer * vertexBuffer,
	GLBuffer * indexBuffer,
	GLVertexArrayObject * vertexArrayObject,
	MeshInfo * outInfo)
{
	attrib_t attributes;
	std::vector<shape_t> shapes;
	std::vector<material_t> materials;
	//Use TinyOBJLoader to load the object
	std::string err;
	bool success = LoadObj(&attributes, &shapes, &materials, &err, fileName.c_str());
	//@TODO: Load more than one mesh./
	if(success)
	{
		bool hasNors =  attributes.normals.size() > 0;
		bool hasTexCoords = attributes.texcoords.size() > 0;
		//Weave the attributes together
		std::vector<float> dataBuffer = std::vector<float>();
		dataBuffer.reserve(attributes.vertices.size() + attributes.normals.size() + attributes.texcoords.size());
		for(int i = 0; i < attributes.vertices.size()/3; i++)
		{
			dataBuffer.push_back(attributes.vertices[i*3]);
			dataBuffer.push_back(attributes.vertices[i*3 + 1]);
			dataBuffer.push_back(attributes.vertices[i*3 + 2]);
			if(hasNors)
			{
				dataBuffer.push_back(attributes.normals[i*3]);
				dataBuffer.push_back(attributes.normals[i*3 + 1]);
				dataBuffer.push_back(attributes.normals[i*3 + 2]);

			}
			if(hasTexCoords)
			{
				dataBuffer.push_back(attributes.texcoords[i*2]);
				dataBuffer.push_back(attributes.texcoords[i*2 + 1]);				
			}
		}
		vertexBuffer->setData(sizeof(float)*dataBuffer.size(), &(dataBuffer[0]), GL_STATIC_DRAW);
		//For now, indices are all shoved into the same
		//array, instead of multiple parts.
		//Assumption: Normal and texCoord indices are 
		//not different than vertex index indices
		std::vector<unsigned short> indices;
		for(const shape_t & shape : shapes)
		{
			for(const index_t & index : shape.mesh.indices )
			{
				indices.push_back((unsigned short)index.vertex_index);
			}
		}
		indexBuffer->setData(sizeof(unsigned short) * indices.size(),&(indices[0]),GL_STATIC_DRAW);

		unsigned dataSize = sizeof(float) * 3;
		intptr_t texCoordsOffset = sizeof(float)*3;
			
		if(hasNors)
		{
			dataSize += sizeof(float)*3;
		}
		if(hasTexCoords)
		{
			dataSize += sizeof(float)*2;
		}

		vertexArrayObject->bindVertexBuffer(
			GL_VERTEX_POSITION_ATTRIBUTE, 
			*vertexBuffer, 
			3,
			GL_FLOAT,
			false,
			dataSize);

		if(hasNors)
		{
			texCoordsOffset += sizeof(float)*3;
			vertexArrayObject->bindVertexBuffer(
			GL_VERTEX_NORMAL_ATTRIBUTE, 
			*vertexBuffer, 
			3,
			GL_FLOAT,
			false,
			dataSize,
			(GLvoid*)(sizeof(float)*3));
		}

		if(hasTexCoords)
		{
			
			vertexArrayObject->bindVertexBuffer(
			GL_VERTEX_NORMAL_ATTRIBUTE, 
			*vertexBuffer, 
			3,
			GL_FLOAT,
			false,
			dataSize,
			(GLvoid*)texCoordsOffset);
		}
		vertexArrayObject->bindElementBuffer(*indexBuffer);
		outInfo -> numVerts = attributes.vertices.size();
		outInfo->numTris = indices.size();
		outInfo->baseVertex = 0;
		outInfo->indexDataOffset = nullptr;
		outInfo->vertexObjectPtr = vertexArrayObject;
	}
	else
	{
		LOG(ERROR, "Could not load mesh named " + fileName);
	}
	return success;
}