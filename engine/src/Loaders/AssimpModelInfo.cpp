#include "AssimpModelInfo.h"
using namespace MoonEngine;
bool AssimpModelInfo::hasNormals() const
{
	return _hasNormals;
}

bool AssimpModelInfo::hasTangentBitangent() const
{
	return _hasTangentBitangent;
}

bool AssimpModelInfo::hasTextureCoordinates() const
{
	return _hasTextureCoordinates;
}

bool AssimpModelInfo::hasBones() const
{
	return _hasBones;
}

const std::vector<AssimpMeshInfo> & AssimpModelInfo::getMeshInfo() const
{
	return _meshInfo;
}

int AssimpModelInfo::stride() const
{
	return (int)hasNormals()* 3 + 
		   (int)hasTangentBitangent() * 6 + 
		   (int)hasTextureCoordinates() * 2;

}


std::vector<std::string> AssimpModelInfo::getAllTextures() const
{
	std::vector<std::string> retVals;
	for(const auto & mesh : _meshInfo)
	{
		for(const auto texStrPair : mesh.textures)
		{
			retVals.push_back(texStrPair.second);
		}
	}
	return retVals;
}

void AssimpModelInfo::addMeshInfo(AssimpMeshInfo & ami)
{
	ami.meshInfo.setVertexArrayObject(vertexObjectPtr);
	_meshInfo.push_back(ami);
}