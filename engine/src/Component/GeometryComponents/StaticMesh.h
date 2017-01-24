#pragma once
#include "Loaders/MeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"
namespace MoonEngine
{


	class StaticMesh : public Mesh
	{
	public:
		StaticMesh(std::string mesh, bool smooth = false);
		
		/**
		 * Bind mesh to the graphics engine.
		 */
		const MeshInfo * getMesh();

	private:
		MeshInfo * _meshInfo;
	};
}
