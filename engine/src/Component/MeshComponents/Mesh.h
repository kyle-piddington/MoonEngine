#pragma once
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"


namespace MoonEngine
{
    class Mesh: public Component
    {
    public:
        Mesh() {}
   		
   		virtual void bind() = 0;    
   		//Get bounding box in world space.
		virtual const BoundingBox & getBoundingBox() = 0;
		//Get bounding box in model space.
		virtual const BoundingBox & getExtents() = 0;

		//Submits an OpenGL call (Don't handle binding/unbinding);
		virtual void draw() const = 0;

		virtual void drawShadow() const{draw();}

    };	
}