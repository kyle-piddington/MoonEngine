#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
	class BeamComponent : public Component
	{
	public:
		BeamComponent(GameObject * pickupObject);
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
		GameObject * moonShardObject;
		GameObject * playerObject;
		glm::vec2 xzPos;
	};
}