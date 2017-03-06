
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"
#include "AssimpMesh.h"
#include "GameObject/GameObject.h"
using namespace MoonEngine;
void AssimpMesh::bind()
{
    _modelInfo->getVAO()->bind();
}

void AssimpMesh::start()
{
    _material = gameObject->getComponent<AssimpMaterial>();
}


        //Get bounding box in world space.
const BoundingBox & AssimpMesh::getBoundingBox()
{
    
    _transformedBox = _modelInfo->boundingBox.transform(gameObject->getTransform().getMatrix());
    return _transformedBox;
}
      //Get bounding box in model space.
const BoundingBox & AssimpMesh::getExtents()
{
    return _modelInfo->boundingBox;
}

void AssimpMesh::draw() const
{
    const std::vector<AssimpMeshInfo> & submeshInfo = _modelInfo->getMeshInfo();
    
    for(int i = 0; i < submeshInfo.size(); i++)
    {
        if(_material != nullptr)
        {
            _material->bindTexturesForMesh(i);
        }
        LOG_GL(__FILE__, __LINE__);
        glDrawElementsBaseVertex(GL_TRIANGLES,
          submeshInfo[i].meshInfo.numTris,
          GL_UNSIGNED_SHORT,
          submeshInfo[i].meshInfo.indexDataOffset,
          submeshInfo[i].meshInfo.baseVertex);
        LOG_GL(__FILE__, __LINE__);
    }
}

void AssimpMesh::drawShadow() const
{
    const std::vector<AssimpMeshInfo> & submeshInfo = _modelInfo->getMeshInfo();
    for(int i = 0; i < submeshInfo.size(); i++)
    {
        glDrawElementsBaseVertex(GL_TRIANGLES,
          submeshInfo[i].meshInfo.numTris,
          GL_UNSIGNED_SHORT,
          submeshInfo[i].meshInfo.indexDataOffset,
          submeshInfo[i].meshInfo.baseVertex);
    }
}

std::shared_ptr<Component> AssimpMesh::clone() const
{
    return std::make_shared<AssimpMesh>(*this);
}