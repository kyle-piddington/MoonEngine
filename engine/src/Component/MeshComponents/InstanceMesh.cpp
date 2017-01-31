#include "InstanceMesh.h"
using namespace MoonEngine;

InstanceMesh::InstanceMesh(std::string mesh, std::string instancedata, bool smooth) :
	Mesh()
{
	vector<glm::mat4> instances;
	GLBuffer* vertexBuffer;
	
	_meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
	instances = EngineApp::GetAssetLibrary().InstTransformLib->getInstanceMatrices(instancedata);
	vertexBuffer->setData(sizeof(float)*instances.size(), &(instances[0]), GL_STATIC_DRAW);

	_meshInfo->getVAO()->bindVertexBuffer(
		GL_VERTEX_INSTANCE_ATTRIBUTE,
		*vertexBuffer,
		16,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 16);
}

const MeshInfo* InstanceMesh::getMesh()
{
	return _meshInfo;
}

std::shared_ptr<Component> InstanceMesh::clone() const
{
	return std::make_shared<InstanceMesh>(*this);
}