#pragma once
#include "Loaders/MeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"
namespace MoonEngine
{
	class InstanceMesh : public Mesh
	{
	public:
		InstanceMesh(std::string mesh, bool smooth);

		/**
		* Bind mesh to the graphics engine.
		*/
		const MeshInfo * getMesh();
		virtual std::shared_ptr<Component> clone() const;

	private:
		MeshInfo * _meshInfo;
	};
}
