#include "ThirdPersonOrbitalController.h"
#include "GameObject/GameObject.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
//Windows C++
#ifndef M_PI
#define M_PI 3.141592653589793
#endif
#include <cmath>
#include <algorithm>

using namespace MoonEngine;

enum CameraStates 
{
	NORMAL,
	ROTATEBACK,
	COLLISIONS
};
ThirdPersonOrbitalController::ThirdPersonOrbitalController(float Cam_Move_Speed, float CamSensitvity) :
	_CamMoveSpeed(Cam_Move_Speed),
	_CamSensitivity(CamSensitvity),
	_phi(0.0f),
	_theta(0.0f),
	_distance(3.0f),
	_state(NORMAL),
	_trac(0.0f, 0.0f, 3.0f),
	_targ(0.0f)
{

}

void ThirdPersonOrbitalController::update(float dt)
{
	glm::vec3 rotate(0.0);

	rotate.y = (Mouse::getLastY() - Mouse::getY())* _CamSensitivity;
	rotate.x = (Mouse::getLastX() - Mouse::getX())* _CamSensitivity;
	_phi += rotate.y;
	_theta += rotate.x;
	_phi = std::min(_phi, (float)M_PI / 2.0f - 0.1f);
	_phi = std::max(_phi, -(float)M_PI / 2.0f + 0.1f);

	rotate.z = 0;

	_trac = _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi), sinf(_phi) * sinf(_theta));

	Transform & transform = gameObject->getTransform();
	transform.setPosition(_trac);
	_targ = transform.getPosition();
	transform.lookAt(_targ);
	
}


std::shared_ptr<Component> ThirdPersonOrbitalController::clone() const
{
	return std::make_shared<ThirdPersonOrbitalController>(*this);
}