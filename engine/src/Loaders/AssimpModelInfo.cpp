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
	return 3 + 
		   (int)hasNormals()* 3 + 
		   (int)hasTextureCoordinates() * 2 + 
		   (int)hasTangentBitangent() * 6 + 
		   (int)hasBones() *  8;

}

int AssimpModelInfo::addBone(const AssimpBoneInfo & boneInfo)
{
	_hasBones = true;
	_boneInfo.push_back(boneInfo);
	_boneToIndexMap[boneInfo.boneName] = _boneInfo.size() - 1;
	return _boneInfo.size() - 1;

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
	boundingBox = boundingBox.merge(ami.box);
}
int AssimpModelInfo::getBoneId(const std::string & string)
{
	const auto & itr = _boneToIndexMap.find(string);
	if(itr != _boneToIndexMap.end())
	{
		return itr->second;
	}
	else
	{
		LOG(ERROR, "Could not find bone of name " + string);
		assert(false);
		return -1;
	}
}
