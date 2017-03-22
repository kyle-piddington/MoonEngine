#pragma once

#include "Component/Components.h"
#include "Component/Component.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
	class PlayerRespawn : public Component
	{
	public:
		PlayerRespawn();

		void respawn();
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:

		glm::vec3 lastPos;
	};
}