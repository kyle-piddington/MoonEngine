#include <Geometry/MeshCreator.h>
#include "MeshLibrary.h"
#include "Loaders/BasicLoader.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Loaders/AssimpLoader.h"
#include "Component/Components.h"

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

BasicMeshInfo * MeshLibrary::getInfoForMeshNamed(std::string meshName, bool smooth)
{
    std::string assembledName = meshName + (smooth ? " _smooth" : "_flat");
    if (_mapMeshToInfo.find(assembledName) == _mapMeshToInfo.end())
    {
        
        //Create and add a new meshInfo
        //(Warn: returning a pointer that i'm not deleting.)
        BasicMeshInfo * info = new BasicMeshInfo;
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

std::shared_ptr<GameObject> MeshLibrary::getGameObjectForModelNamed(std::string name, std::string program, Scene * scene)
{
    Scene * scnPtr = scene;
    if(scnPtr == nullptr)
    {
        scnPtr = GetWorld();
    }
    //Check to make sure we have a scene.
    assert(scnPtr != nullptr);
    std::shared_ptr<AssimpModelInfo>  info;
    if (assimpInfo.find(name) == assimpInfo.end())
    {
        _meshBuffers.push_back(std::make_shared<GLBuffer>(GL_ARRAY_BUFFER));
        _meshBuffers.push_back(std::make_shared<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER));
        GLBuffer * vertBuffer = _meshBuffers[_meshBuffers.size() - 2].get();
        GLBuffer * indBuffer = _meshBuffers[_meshBuffers.size() - 1].get();
        _meshVAOs.push_back(std::make_shared<GLVertexArrayObject>());
        GLVertexArrayObject * vao = _meshVAOs[_meshVAOs.size() - 1].get();
        info = std::make_shared<AssimpModelInfo>();
        info->setHasNormals(true);
        info->setHasTangentBitangent(false);
        info->setHasTextureCoordinates(true);
        AssimpLoader::LoadIntoBuffer(_recPath + name,vertBuffer,indBuffer,vao,
        info.get());
        assimpInfo[name] = info;

    }
    else
    {
        info = assimpInfo[name];
    }
    std::shared_ptr<GameObject> object = scene->createGameObject();
    object->addComponent(scene->createComponent<AssimpMaterial>(program, info.get()));
    object->addComponent(scene->createComponent<AssimpMesh>(info.get()));
    return object;

}

void MeshLibrary::loadDefaultMesh() {
    _mapMeshToInfo["quad_flat"] = MeshCreator::CreateQuad(glm::vec2(-0.5, -0.5), glm::vec2(0.5, 0.5));
    _mapMeshToInfo["quad_full_flat"] = MeshCreator::CreateQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
}