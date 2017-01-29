#include "ThirdPersonCharacterController.h"
#include "Component/CameraComponents/Camera.h"
#include "IO/Keyboard.h"
#include <glfw/glfw3.h>
#include "IO/Input.h"
#include "GlobalFuncs/Instantiate.h"
#include "thirdparty/imgui/imgui.h"
using namespace MoonEngine;
ThirdPersonCharacterController::ThirdPersonCharacterController(float playerSpeed)
{
	this->playerSpeed = playerSpeed;
	gravity = -9.8;
	jumpForce = 3;
	jumpTime = 0.15f; //Half a second of 'up' time
	state = GROUND;

}

void ThirdPersonCharacterController::start()
{
	mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();
	transform = &gameObject->getTransform();
}

void ThirdPersonCharacterController::update(float dt)
{
	handleMove(dt);
	handleJump(dt);

}

void ThirdPersonCharacterController::handleMove(float dt)
{
	glm::vec3 camForward = mainCamera->getTransform().forward();
	glm::vec3 camForwardXZ = glm::vec3(camForward.x,0,camForward.z);
	glm::vec3 camRight= mainCamera->getTransform().right();
	glm::vec3 camRightXZ = -glm::vec3(camRight.x,0,camRight.z);
	
	glm::vec2 direction = glm::vec2(Input::GetAxis(AXIS_HORIZONTAL_0),Input::GetAxis(AXIS_VERTICAL_0));
	glm::vec3 playerDirection = (camForwardXZ * direction.y + camRightXZ * direction.x);
	//Look along movement
	if(glm::length(playerDirection) > 2e-1)
	{
		transform->lookAt(transform->getPosition() + playerDirection);
	}
	transform->translate(playerSpeed * dt * playerDirection );
	
}

void ThirdPersonCharacterController::handleJump(float dt)
{
	//GUI
	ImGui::Begin("Jump Physics");
	ImGui::InputFloat( "Jump force", &jumpForce);
	ImGui::InputFloat( "Jump Time", &jumpTime);
	ImGui::InputFloat( "Gravity",&gravity);
	ImGui::Value( "Jump Speed",(float)jumpSpeed);
	ImGui::Value( "Jump Time",(float)_jumpTime);
	
	ImGui::End();

	//END GUI
	if(Input::GetButtonDown(BUTTON_0) && state == GROUND)
	{
		LOG(GAME,"STARTING JUMP");
		_curJumpForce = jumpForce;
		_jumpTime = jumpTime;
		state = JUMPING;
	}
	else if(Input::GetButton(BUTTON_0) && state == JUMPING && _jumpTime > 0)
	{
	
		
		_jumpTime -= dt;
	}
	else if(state == JUMPING)
	{
		LOG(GAME, "ENDING JUMP");
		state = FALLING;
	}
	if(state == JUMPING)
	{
		jumpSpeed = _curJumpForce;
		transform->translate(glm::vec3(0,jumpSpeed*dt,0));
	}
	else if(state == FALLING)
	{
		jumpSpeed += dt *  gravity;
		transform->translate(glm::vec3(0,jumpSpeed*dt,0));
	}
	else
	{
		jumpSpeed = 0;
	}
	if(transform->getPosition().y <= 0 && state == FALLING){
		transform->setPosition(
			glm::vec3(transform->getPosition().x,0,transform->getPosition().z));
		state = GROUND;
	}

}

void ThirdPersonCharacterController::onCollisionEnter(Collision col)
{
	LOG(GAME, "Normal is" + std::to_string(col.normal.x) + " " + std::to_string(col.normal.y) + " " + std::to_string(col.normal.z));
	if((state == FALLING || state == JUMPING) && glm::dot(col.normal, glm::vec3(0,1,0)) > cosf(M_PI/3))
	{
		LOG(GAME, "Hit ground");
		state = GROUND;		
	}
}

std::shared_ptr<Component> ThirdPersonCharacterController::clone() const
{
	return std::make_shared<ThirdPersonCharacterController>(*this);
}