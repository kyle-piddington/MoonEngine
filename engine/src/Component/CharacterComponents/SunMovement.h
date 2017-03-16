#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"
#include "Component/LightComponents/DirLight.h"

namespace MoonEngine
{
	class SunMovement : public Component
	{
	public:
		SunMovement();
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
		DirLight * dirLightObject;
		GameObject * playerObj;
	};

}