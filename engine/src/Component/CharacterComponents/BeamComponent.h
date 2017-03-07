#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
	class BeamComponent : public Component
	{
	public:
		BeamComponent();
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
		GameObject * playerObject;
		glm::vec2 xzPos;
	};
}