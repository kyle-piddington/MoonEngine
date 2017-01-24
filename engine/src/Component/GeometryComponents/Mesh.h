#pragma once
/**
 * Virtual mesh interface for any
 * renderable meshes
 */
#include "Component/Component.h"
#include "Loaders/MeshInfo.h"
namespace MoonEngine
{
	class Mesh : public Component
	{
	public:
		Mesh()
		{

		}
		virtual const MeshInfo * getMesh() = 0;
		virtual std::shared_ptr<Component> clone() const = 0;

	};
}