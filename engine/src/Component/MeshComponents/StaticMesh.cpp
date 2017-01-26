#include "StaticMesh.h"
#include "EngineApp/EngineApp.h"
using namespace MoonEngine;

StaticMesh::StaticMesh(std::string mesh, bool smooth):
	Mesh()
{
	_meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
}

const MeshInfo * StaticMesh::getMesh()
{
	return _meshInfo;
}

std::shared_ptr<Component> StaticMesh::clone() const
{
	return std::make_shared<StaticMesh>(*this);
}