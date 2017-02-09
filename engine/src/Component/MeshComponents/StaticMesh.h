#pragma once

#include "Loaders/MeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"

namespace MoonEngine
{
    class StaticMesh: public Mesh
    {
    public:
        StaticMesh(std::string mesh, bool smooth = false);

        StaticMesh(MeshInfo * _meshInfo);
        
        void start();

        /**
         * Bind mesh to the graphics engine.
         */
        const MeshInfo * getMesh();
        

        const BoundingBox & getBoundingBox();

        std::shared_ptr<Component> clone() const;

		void draw() const;
    private:
        MeshInfo * _meshInfo;
        BoundingBox bBox;
    };

}
