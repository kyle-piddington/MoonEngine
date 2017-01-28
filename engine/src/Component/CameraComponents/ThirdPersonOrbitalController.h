#pragma once

#include "Component/Component.h"

/**
* Creates a third person orbital camera that follows the player.
*/
namespace MoonEngine
{
	class ThirdPersonOrbitalController : public Component
	{
	public:
		ThirdPersonOrbitalController(float Cam_Move_Speed = 1, float CamSensitivity = 0.002);
		void start();
		void update(float dt);
		std::shared_ptr<Component> clone() const;
	private:
		GameObject * player;
		float _CamMoveSpeed;
		float _CamSensitivity;
		glm::vec3 _trac;
		glm::vec3 _targ;
		glm::vec3 _tracInterp;
		float _phi;
		float _theta;
		float _distance;
		int _state;
	};
}

