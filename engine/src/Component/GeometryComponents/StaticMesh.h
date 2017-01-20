#pragma once
#include "Loaders/MeshInfo.h"
#include "Component/Component.h"
namespace MoonEngine
{


	class StaticMesh : public Component
	{
		StaticMesh(std::shared_ptr<GameObject> object, std::string mesh);
		
		/**
		 * Bind mesh to the graphics engine.
		 */
		void bind();

	private:
		MeshInfo * meshInfo;
	};
}
