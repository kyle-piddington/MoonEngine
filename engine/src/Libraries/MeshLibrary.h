#pragma once
/**
 * MeshLibrary provides a mapping of 
 * mesh names to vertex array objects. 
 */
#include <string>
#include <unordered_map>
#include <vector>
#include "GLWrapper/GLVertexArrayObject.h"
#include "Loaders/BasicMeshInfo.h"
#include <memory>
#include "Loaders/AssimpModelInfo.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

namespace MoonEngine
{
    class MeshLibrary
    {
    public:
        MeshLibrary(std::string resourcePath);

        ~MeshLibrary();

        BasicMeshInfo * getInfoForMeshNamed(std::string mesh, bool smooth = false);
        /**
         * Assemble a game object for a model
         * @param  mesh          Resource path of the model
         * @param  renderProgram Program to bind to the material
         * @return               A configured gameObject.
         */
        std::shared_ptr<GameObject> getGameObjectForModelNamed(std::string mesh, std::string renderProgram, Scene * scene = nullptr);
    private:
        std::string _recPath;
        std::unordered_map<std::string, BasicMeshInfo *> _mapMeshToInfo;
        std::vector<std::shared_ptr<GLVertexArrayObject>> _meshVAOs;
        std::vector<std::shared_ptr<GLBuffer>> _meshBuffers;
        std::unordered_map<std::string, std::shared_ptr<AssimpModelInfo>> assimpInfo;
        
        void loadDefaultMesh();
    };
}