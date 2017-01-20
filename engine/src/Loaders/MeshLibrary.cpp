#include "MeshLibrary.h"
#include "BasicLoader.h"
using namespace MoonEngine;

MeshLibrary::MeshLibrary(std::string resourcePath):
	_recPath(resourcePath + "/")
	{

	}
MeshLibrary::~MeshLibrary()
{
	for(auto & mapRes: _mapMeshToInfo)
	{
		delete(mapRes.second);
	}
}

MeshInfo * MeshLibrary::getInfoForMeshNamed(std::string meshName)
{
	if(_mapMeshToInfo.find(meshName) == _mapMeshToInfo.end())
	{
		//Create and add a new meshInfo
		MeshInfo * info = new MeshInfo;
		_meshBuffers.push_back(GLBuffer(GL_ARRAY_BUFFER));
		_meshBuffers.push_back(GLBuffer(GL_ELEMENT_ARRAY_BUFFER));
		
		GLBuffer & vertBuffer = _meshBuffers[_meshBuffers.size() - 2];
		GLBuffer & indBuffer = _meshBuffers[_meshBuffers.size() - 1];
		_meshVAOs.push_back(GLVertexArrayObject());
		GLVertexArrayObject & vao = _meshVAOs.back();
		BasicLoader::LoadIntoBuffer(_recPath + meshName, &vertBuffer, &indBuffer, &vao, info);
		_mapMeshToInfo[meshName] = info;
			
	}

	return _mapMeshToInfo[meshName];
	
}