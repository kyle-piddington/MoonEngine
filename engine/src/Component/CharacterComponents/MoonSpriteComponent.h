#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"
#include "Component/LightComponents/DirLight.h"

namespace MoonEngine
{
	class MoonSpriteComponent : public Component
	{
	public:
		MoonSpriteComponent();

		void start();

		void update(float dt);

		std::shared_ptr<Component> clone() const;

	private:
		float _raiseTime;
		DirLight * dirLightObject;
		GameObject * playerObj;
	};

}