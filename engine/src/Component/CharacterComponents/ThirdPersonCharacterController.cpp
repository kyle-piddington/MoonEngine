#include "ThirdPersonCharacterController.h"
#include "Component/CameraComponents/Camera.h"
#include "IO/Keyboard.h"
#include <glfw/glfw3.h>
#include "IO/Input.h"
#include "GlobalFuncs/Instantiate.h"
using namespace MoonEngine;
ThirdPersonCharacterController::ThirdPersonCharacterController(float playerSpeed)
{
	this->playerSpeed = playerSpeed;

}

void ThirdPersonCharacterController::start()
{
	mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();
	transform = &gameObject->getTransform();
}

void ThirdPersonCharacterController::update(float dt)
{
	glm::vec3 camForward = mainCamera->getTransform().forward();
	glm::vec3 camForwardXZ = glm::vec3(camForward.x,0,camForward.z);
	glm::vec3 camRight= mainCamera->getTransform().right();
	glm::vec3 camRightXZ = -glm::vec3(camRight.x,0,camRight.z);
	
	glm::vec2 direction = glm::vec2(Input::GetAxis(AXIS_HORIZONTAL_0),Input::GetAxis(AXIS_VERTICAL_0));
	glm::vec3 playerDirection = (camForwardXZ * direction.y + camRightXZ * direction.x);
	//Look along movement
	if(glm::length(playerDirection) > 1e-2)
	{
		transform->lookAt(transform->getPosition() + playerDirection);
	}
	transform->translate(playerSpeed * dt * playerDirection );
	if(Keyboard::isKeyDown(GLFW_KEY_SPACE))
	{
		transform->translate(glm::vec3(0,playerSpeed*dt,0));
	}
	else if(transform->getPosition().y > 0.5)
	{
		transform->translate(glm::vec3(0,-playerSpeed*dt,0));	
	}
}

std::shared_ptr<Component> ThirdPersonCharacterController::clone() const
{
	return std::make_shared<ThirdPersonCharacterController>(*this);
}