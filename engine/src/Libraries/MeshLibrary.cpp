#include <Geometry/MeshCreator.h>
#include "MeshLibrary.h"
#include "Loaders/BasicLoader.h"

using namespace MoonEngine;

MeshLibrary::MeshLibrary(std::string resourcePath):
    _recPath(resourcePath)
{
    loadDefaultMesh();
}

MeshLibrary::~MeshLibrary()
{
    for (auto & mapRes: _mapMeshToInfo)
    {
        delete (mapRes.second);
    }
}

MeshInfo * MeshLibrary::getInfoForMeshNamed(std::string meshName, bool smooth)
{
    std::string assembledName = meshName + (smooth ? " _smooth" : "_flat");
    if (_mapMeshToInfo.find(assembledName) == _mapMeshToInfo.end())
    {
        //Create and add a new meshInfo
        //(Warn: returning a pointer that i'm not deleting.)
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

void MeshLibrary::loadDefaultMesh() {
    _mapMeshToInfo["quad_flat"] = MeshCreator::CreateQuad(glm::vec2(-0.5, -0.5), glm::vec2(0.5, 0.5));
    _mapMeshToInfo["quad_full"] = MeshCreator::CreateQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
}