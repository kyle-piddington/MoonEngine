#pragma once

#include "Loaders/BasicMeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"
#include "EngineApp/EngineApp.h"
#include "GLWrapper/GLConstants.h"

namespace MoonEngine
{
    class InstanceMesh: public Mesh
    {
    public:
        InstanceMesh(std::string mesh, std::string instancedata, bool smooth);

        /**
        * Bind mesh to the graphics engine.
        * Bind instance data to the graphics engines
        */
        void bind();

        virtual std::shared_ptr<Component> clone() const;

		virtual const BoundingBox & getBoundingBox();
        virtual const BoundingBox & getExtents(){return getBoundingBox();}

        void draw() const;

    private:
        BasicMeshInfo * _meshInfo;

		unsigned _numOfInstances;
	
		std::vector<glm::mat4> _instanceTransforms;

    };

}
