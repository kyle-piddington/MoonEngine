#include "MeshLibrary.h"
#include "BasicLoader.h"
#include "Util/Logger.h"
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

MeshInfo * MeshLibrary::getInfoForMeshNamed(std::string meshName, bool smooth)
{
	std::string assembledName = meshName + (smooth ? " _smooth" : "_flat");
	if(_mapMeshToInfo.find(assembledName) == _mapMeshToInfo.end())
	{
		//Create and add a new meshInfo
		MeshInfo * info = new MeshInfo;
		_meshBuffers.push_back(std::make_shared<GLBuffer>(GL_ARRAY_BUFFER));
		_meshBuffers.push_back(std::make_shared<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER));
		
		GLBuffer * vertBuffer = _meshBuffers[_meshBuffers.size() - 2].get();
		GLBuffer * indBuffer = _meshBuffers[_meshBuffers.size() - 1].get();
		_meshVAOs.push_back(std::make_shared<GLVertexArrayObject>());
		GLVertexArrayObject * vao = _meshVAOs[_meshVAOs.size() - 1].get();
		
		BasicLoader::LoadIntoBuffer(_recPath + meshName, vertBuffer, indBuffer, vao, info, smooth);
		_mapMeshToInfo[assembledName] = info;
			
	}

	return _mapMeshToInfo[assembledName];
	
}