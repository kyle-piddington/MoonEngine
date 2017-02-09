#include "InstanceMesh.h"

using namespace MoonEngine;


InstanceMesh::InstanceMesh(std::string mesh, std::string instancedata, bool smooth):
        Mesh()
{
    vector<glm::mat4> instances;

    _meshInfo = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed(mesh, smooth);
    instances = EngineApp::GetAssetLibrary().InstTransformLib->getInstanceMatrices(instancedata);
    GLBuffer vertexBuffer(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.size(), &instances[0], GL_STATIC_DRAW);
    _numOfInstances = instances.size();

    _meshInfo->getVAO()->bindVertexBuffer(GL_VERTEX_INSTANCE_MATRIX_ATTRIBUTE, vertexBuffer,
        sizeof(glm::vec4), GL_FLOAT, GL_FALSE, sizeof(glm::vec4));
}

const MeshInfo * InstanceMesh::getMesh()
{
    return _meshInfo;
}

const BoundingBox & InstanceMesh::getBoundingBox()
{
	return _meshInfo->boundingBox;
}



std::shared_ptr<Component> InstanceMesh::clone() const
{
	return std::make_shared<InstanceMesh>(*this);
}

void InstanceMesh::draw() const
{
    glDrawElementsInstanced(GL_TRIANGLES,
        _meshInfo->numTris,
        GL_UNSIGNED_SHORT,
        _meshInfo->indexDataOffset,
        _numOfInstances);   
}