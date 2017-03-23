#pragma once
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"
#include "Loaders/AssimpModelInfo.h"
#include "Component/MaterialComponents/AssimpMaterial.h"
#include "Mesh.h"

namespace MoonEngine
{
  class AssimpMesh: public Mesh
  {
  public:
    AssimpMesh(AssimpModelInfo * info):
    _modelInfo(info) {}

    void start();

    virtual void bind();    
     		//Get bounding box in world space.
    virtual const BoundingBox & getBoundingBox();
  		//Get bounding box in model space.
    virtual const BoundingBox & getExtents();

    std::shared_ptr<Component> clone() const;
  		//Submits an OpenGL call (Don't handle binding/unbinding);
    virtual void draw() const;

	virtual void drawShadow() const;

    
  private:
    AssimpModelInfo * _modelInfo;
    AssimpMaterial *  _material;
	
    BoundingBox _transformedBox;
  };	
}