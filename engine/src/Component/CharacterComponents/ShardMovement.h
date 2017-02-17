#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
	class ShardMovement : public Component
	{
	public:
		ShardMovement();
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
		float startingTime;
	};
}