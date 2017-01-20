#pragma once
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"
namespace MoonEngine
{
	class Mesh : public Component
	{
	public:
		Mesh(std::shared_ptr<GameObject> obj):
			Component(obj)
		{

		}
		virtual const MeshInfo * getMesh() = 0;
	};
}