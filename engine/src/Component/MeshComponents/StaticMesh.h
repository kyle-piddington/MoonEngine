#pragma once

#include "Loaders/BasicMeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"

namespace MoonEngine
{
    class StaticMesh: public Mesh
    {
    public:
        StaticMesh(std::string mesh, bool smooth = false);

        StaticMesh(BasicMeshInfo * _meshInfo);
        
        void start();

        /**
         * Bind mesh to the graphics engine.
         */
        void draw();

        void bind();
        

        const BoundingBox & getBoundingBox();

        const BoundingBox & getExtents();

        std::shared_ptr<Component> clone() const;

		void draw() const;
    private:
        BasicMeshInfo * _meshInfo;
        BoundingBox bBox;
    };

}
