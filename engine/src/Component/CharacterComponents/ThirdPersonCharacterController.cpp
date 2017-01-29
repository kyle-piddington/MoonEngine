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
	radius = 0;
	state = GROUND;

}

void ThirdPersonCharacterController::start()
{
	mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();
	transform = &gameObject->getTransform();
	bbox = gameObject->getComponent<BoxCollider>();
	
}

void ThirdPersonCharacterController::update(float dt)
{
	if(radius == 0)
	{
		radius = std::max(std::max(bbox->getHalfWidths().x, bbox->getHalfWidths().y),bbox->getHalfWidths().z);
		LOG(GAME, "Radius: " + std::to_string(radius));
	}
	checkIfShouldFall();
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
	//direction.y = -direction.y;
	glm::vec3 playerDirection = (camForwardXZ * direction.y + camRightXZ * direction.x);
	
	//Look along movement
	if(glm::length(playerDirection) > 2e-1)
	{
		transform->lookAt(transform->getPosition() - playerDirection);
	}
	Hit moveHit;
	playerDirection *= playerSpeed;
	LOG(GAME, "playerDirection: " + std::to_string(playerDirection.x) +"," + std::to_string(playerDirection.y));
	
	LOG(GAME, "transform: " + std::to_string(transform->forward().x) +"," + std::to_string(transform->forward().y)+"," + std::to_string(transform->forward().z));
	//Check and remove movement along colliding axis.
	if(GetWorld()->castRay(transform->getPosition(),transform->forward(),radius, &moveHit))
	{
		LOG(GAME, "RayHit");
		if(glm::dot(moveHit.intersectionNormal, playerDirection) < 0)
		{
			playerDirection -= moveHit.intersectionNormal * glm::dot(playerDirection , moveHit.intersectionNormal);			
		}

	}
	transform->translate( dt * playerDirection );
	
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
	if(transform->getPosition().y <= bbox->getHalfWidths().y && state == FALLING){
		transform->setPosition(
			glm::vec3(transform->getPosition().x,bbox->getHalfWidths().y,transform->getPosition().z));
		state = GROUND;
	}

}

void ThirdPersonCharacterController::onCollisionEnter(Collision col)
{
	if((state == FALLING || state == JUMPING) && glm::dot(col.normal, glm::vec3(0,1,0)) > cosf(M_PI/3))
	{
		LOG(GAME, "Hit ground");
		state = GROUND;		
	}
}


void ThirdPersonCharacterController::checkIfShouldFall()
{
	//Early break if on ground.
	if(transform->getPosition().y <= bbox->getHalfWidths().y)
	{
	
		return;
	}
	if(state != JUMPING && !GetWorld()->castRay(transform->getPosition(),glm::vec3(0,-1,0),bbox->getHalfWidths().y))
	{
		state = FALLING;
	}
}
std::shared_ptr<Component> ThirdPersonCharacterController::clone() const
{
	return std::make_shared<ThirdPersonCharacterController>(*this);
}