#pragma once
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"
#include "Loaders/MeshInfo.h"

namespace MoonEngine
{
    class Mesh: public Component
    {
    public:
        Mesh() {}

        virtual const MeshInfo * getMesh() = 0;


		virtual const BoundingBox & getBoundingBox() = 0;

		//Submits an OpenGL call (Don't handle binding/unbinding);
		virtual void draw() const = 0;
    };
}