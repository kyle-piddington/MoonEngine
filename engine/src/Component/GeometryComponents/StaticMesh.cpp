#include "StaticMesh.h"
#include "EngineApp/EngineApp.h"
using namespace MoonEngine;

StaticMesh::StaticMesh(std::shared_ptr<GameObject> object, std::string mesh):
	Mesh(object)
{
	_meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh);
}

const MeshInfo * StaticMesh::getMesh()
{
	return _meshInfo;
}
