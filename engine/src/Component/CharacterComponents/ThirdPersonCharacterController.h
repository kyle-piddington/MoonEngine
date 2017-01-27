#pragma once

#include "Component/Component.h"
#include "Geometry/Transform.h"
/**
 * Controls a character in a first-person matter
 */
namespace MoonEngine
{


	class ThirdPersonCharacterController : public Component
	{
	public:
		ThirdPersonCharacterController(float playerSpeed = 1);
		void start();
		void update(float dt);
		std::shared_ptr<Component> clone() const;

	private:
		Transform * transform;
		float playerSpeed;
		GameObject * mainCamera;


	};
}