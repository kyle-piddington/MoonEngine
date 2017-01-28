#include "ThirdPersonOrbitalController.h"
#include "GameObject/GameObject.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/Input.h"
#include "Util/Logger.h"
#include "GlobalFuncs/Instantiate.h"
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
	_phi(M_PI/6),
	_theta(0.0f),
	_distance(3.0f),
	_state(NORMAL),
	_trac(0.0f, 0.0f, 3.0f),
	_targ(0.0f)
{

}

void ThirdPersonOrbitalController::start()
{
	_trac = _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi), sinf(_phi) * sinf(_theta));
	Transform & transform = gameObject->getTransform();
	transform.setPosition(_trac);
	transform.lookAt(_targ);
	player = GetWorld()->findGameObjectWithTag(T_Player);
	assert(player != nullptr);
	//Look for player by default
	//
	LOG(GAME, "Camera started");
}
void ThirdPersonOrbitalController::update(float dt)
{
	glm::vec3 rotate(0.0);

	rotate.y = (Mouse::getLastY() - Mouse::getY())* _CamSensitivity;
	rotate.x = (Mouse::getLastX() - Mouse::getX())* _CamSensitivity;
	rotate.y += Input::GetAxis(AXIS_VERTICAL_1)*_CamMoveSpeed*dt;
    rotate.x += Input::GetAxis(AXIS_HORIZONTAL_1)*_CamMoveSpeed*dt;
   
	_phi += rotate.y;
	_theta += rotate.x;
	_phi = std::min(_phi, (float)M_PI / 2.0f - 0.1f);
	_phi = std::max(_phi, -(float)M_PI / 2.0f + 0.1f);

	rotate.z = 0;
	_targ = player->getTransform().getPosition();
	_trac =  _targ + _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi), sinf(_phi) * sinf(_theta));
	Transform & transform = gameObject->getTransform();
	transform.setPosition(_trac);
	//_targ = transform.getPosition();
	transform.lookAt(_targ);
	
}


std::shared_ptr<Component> ThirdPersonOrbitalController::clone() const
{
	return std::make_shared<ThirdPersonOrbitalController>(*this);
}