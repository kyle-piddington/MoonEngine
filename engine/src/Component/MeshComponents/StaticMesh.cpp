#include "StaticMesh.h"
#include "EngineApp/EngineApp.h"

using namespace MoonEngine;

StaticMesh::StaticMesh(std::string mesh, bool smooth):
    Mesh()
{
    _meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
}

StaticMesh::StaticMesh(BasicMeshInfo * _meshInfo):
    Mesh(),
    _meshInfo(_meshInfo)
{
}

void StaticMesh::start()
{
	bBox =
		 _meshInfo->boundingBox.transform(gameObject->getTransform().getMatrix());
}
const BoundingBox & StaticMesh::getExtents()
{
	return _meshInfo->boundingBox;
}

const BoundingBox & StaticMesh::getBoundingBox()
{
	bBox =
		 _meshInfo->boundingBox.transform(gameObject->getTransform().getMatrix());
	return bBox;
}

void StaticMesh::bind()
{
    _meshInfo->bind();
}

std::shared_ptr<Component> StaticMesh::clone() const
{
	return std::make_shared<StaticMesh>(*this);
}
void StaticMesh::draw() const
{

	glDrawElementsBaseVertex(GL_TRIANGLES,
				_meshInfo->numTris,
				GL_UNSIGNED_SHORT,
				_meshInfo->indexDataOffset,
				_meshInfo->baseVertex);
}