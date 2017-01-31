#pragma once
#include "Loaders/MeshInfo.h"
#include "Component/Component.h"
#include "Mesh.h"
#include "EngineApp/EngineApp.h"
#include "GLWrapper/GLConstants.h"

namespace MoonEngine
{
	class InstanceMesh : public Mesh
	{
	public:
		InstanceMesh(std::string mesh, std::string instancedata, bool smooth);

		/**
		* Bind mesh to the graphics engine.
		* Bind instance data to the graphics engines
		*/
		const MeshInfo * getMesh();
		virtual std::shared_ptr<Component> clone() const;

	private:
		MeshInfo * _meshInfo;
		std::vector<glm::mat4> _instanceTransforms;
	};
}
