#pragma once

#include "Loaders/BasicMeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"
#include "EngineApp/EngineApp.h"
#include "GLWrapper/GLConstants.h"
#include "Component/MeshComponents/Terrain.h"

namespace MoonEngine
{
    class Grass: public Mesh
    {
    public:
        Grass(std::string mesh, bool smooth, unsigned maxInstances);

        /**
        * Bind mesh to the graphics engine.
        * Bind instance data to the graphics engines
        */
        void bind();

        void start();

        void update(float dt);

        virtual std::shared_ptr<Component> clone() const;

		virtual const BoundingBox & getBoundingBox();

		virtual const BoundingBox & getExtents();

		void addTransformsToBuffer(const std::vector<glm::mat4> & newTransforms);

        void draw() const;

        //No shadows from grass
        void drawShadow() const {}


    private:

    	void generateGrassForTile(CDLODQuadtree::SelectedNode & node, std::vector<glm::mat4> * newTransforms);
    	float windTime;
        BasicMeshInfo * _meshInfo;
		unsigned _numOfInstances;
		unsigned _cInstance;
		unsigned _maxInstances;
		//Buffer used as a ring buffer to update grass.
	    std::shared_ptr<GLBuffer> _instanceBuffer;
	    Terrain * _terrainComponent;
	    GameObject * player;
	    BoundingBox bBox;
		std::vector<glm::mat4> _instanceTransforms;

    };

}
