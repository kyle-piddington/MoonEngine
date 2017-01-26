#include "InstanceMesh.h"
#include "EngineApp/EngineApp.h"
using namespace MoonEngine;

InstanceMesh::InstanceMesh(std::string mesh, bool smooth) :
	Mesh()
{
	_meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
}

const MeshInfo* InstanceMesh::getMesh()
{
	return _meshInfo;
}

std::shared_ptr<Component> InstanceMesh::clone() const
{
	return std::make_shared<InstanceMesh>(*this);
}