#include "StaticMesh.h"
#include "EngineApp/EngineApp.h"
using namespace MoonEngine;

StaticMesh::StaticMesh(std::string mesh):
	Mesh()
{
	_meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh);
}

const MeshInfo * StaticMesh::getMesh()
{
	return _meshInfo;
}
