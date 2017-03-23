#include "ThirdPersonCharacterController.h"
#include "Component/CameraComponents/Camera.h"
#include "IO/Keyboard.h"
#include <glfw/glfw3.h>
#include "IO/Input.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "thirdparty/imgui/imgui.h"
#include <string>
#include <algorithm>
#include <MoonEngine.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif
using namespace MoonEngine;

ThirdPersonCharacterController::ThirdPersonCharacterController(float playerSpeed)
{
    this->playerSpeed = playerSpeed;
    gravity = -9.8;
    jumpForce = 3;
    _jumpTime = jumpTime = 0.15f; //Half a second of 'up' time
    jumpSpeed = 0;
    radius = 0;
    state = GROUND;
    _lastGround = 0;
    _curJumpForce = 0;
    playerSpeed = 0;
    _currentAnim = ANIM_SEAT;
	numWisps = 0;
	wispTime = 0.0f;
}

void ThirdPersonCharacterController::start()
{
    mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();
    transform = &gameObject->getTransform();
    bbox = gameObject->getComponent<BoxCollider>();
    GameObject * worldTerrainGO = GetWorld()->findGameObjectWithComponent<Terrain>();
    if(worldTerrainGO != nullptr)
    {
        worldTerrain = worldTerrainGO->getComponent<Terrain>();
    }
    else
    {
        worldTerrain = nullptr;
    }
    animator = gameObject->getComponent<Animator>();
    if(animator)
    {
        animator->setAnimation(animations[_currentAnim]);
    }
    findMinGround();
	GetWorld()->instantiate(GetWorld()->getPrefab("Wisp").get(), gameObject->getTransform());
}

void ThirdPersonCharacterController::update(float dt)
{
    if (radius == 0)
    {
        radius = std::max(std::max(bbox->getHalfWidths().x, bbox->getHalfWidths().y), bbox->getHalfWidths().z);
        LOG(GAME, "Radius: " + std::to_string(radius));
    }
    findMinGround();
    checkIfShouldFall();
    handleMove(dt);
    handleJump(dt);
	
	if (numWisps < 20)
	{
		wispTime += dt;
		if (wispTime >= 0.2f)
		{
			wispTime = 0.0f;
			GetWorld()->instantiate(GetWorld()->getPrefab("Wisp").get(), gameObject->getTransform());
			numWisps++;
		}
	}
	
}

void ThirdPersonCharacterController::findMinGround()
{
    if(worldTerrain != nullptr)
    {
        glm::vec3 pos = gameObject->getTransform().getPosition();
        _lastGround = worldTerrain->heightAt(pos.x,pos.z);
    }
    else
    {
        _lastGround = 0;
    }
    Hit h;
    if(GetWorld()->castRay(transform->getPosition(), glm::vec3(0,-1,0),FLT_MAX,&h, T_Player))
    {
        _lastGround = std::max(_lastGround, h.intersectionPoint.y);
    }

    

}
void ThirdPersonCharacterController::handleMove(float dt)
{
    glm::vec3 camForward = mainCamera->getTransform().forward();
    glm::vec3 camForwardXZ = glm::normalize(glm::vec3(camForward.x, 0, camForward.z));
    glm::vec3 camRight = mainCamera->getTransform().right();
    glm::vec3 camRightXZ = -glm::normalize(glm::vec3(camRight.x, 0, camRight.z));


    glm::vec2 direction = glm::vec2(Input::GetAxis(AXIS_HORIZONTAL_0), Input::GetAxis(AXIS_VERTICAL_0));

    /* Change animation state? */
    int anim;
    glm::vec2 dir = abs(direction);

    if (glm::all(glm::lessThan(dir, glm::vec2(0.001, 0.001))))
    {
        anim = ANIM_IDLE;
    } else if (glm::all(glm::lessThan(dir, glm::vec2(0.6, 0.6))))
    {
        anim = ANIM_WALK;
    } else
    {
        anim = ANIM_RUN;
    }
    if (Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::GetButton(BUTTON_1))
    {
        anim = ANIM_CREEP;
    }

    if (anim != _currentAnim) {
        _currentAnim = anim;
        if (animator)
        {
            animator->setAnimation(animations[_currentAnim]);
        }
    }

    //direction.y = -direction.y;
    glm::vec3 playerDirection = (camForwardXZ * direction.y + camRightXZ * direction.x);

    //Look along movement
    if (glm::length(playerDirection) > 2e-1)
    {
        transform->lookAt(transform->getPosition() - playerDirection);
    }
    Hit moveHit;
    playerDirection *= playerSpeed;
    //LOG(GAME, "playerDirection: " + std::to_string(playerDirection.x) +"," + std::to_string(playerDirection.y));
    //LOG(GAME, "transform: " + std::to_string(transform->forward().x) +"," + std::to_string(transform->forward().y)+"," + std::to_string(transform->forward().z));
    //Check and remove movement along colliding axis.
    if (GetWorld()->castRay(transform->getPosition(), transform->forward(), radius + 0.1f, &moveHit, T_Player))
    {
        //LOG(GAME, "RayHit");
        if (glm::dot(moveHit.intersectionNormal, playerDirection) < 0)
        {
            playerDirection -= moveHit.intersectionNormal * glm::dot(playerDirection, moveHit.intersectionNormal);
        }

    }
    glm::vec3 slideDirection = checkIfShouldSlide();
    if(state != SLIDING)
    {
		if (slopeOK(dt * playerDirection))
		{
			transform->translate(dt * playerDirection);
		}

    }
    else
    {

        glm::vec3 grav = glm::vec3(0,dt * jumpSpeed,0);
        float pen = std::min(dt * 8.1f, abs(dt * jumpSpeed));
        glm::vec3 reflect = slideDirection * pen;
        reflect.y = 0;
        transform->translate(grav + reflect);
    }
    
    if(_lastGround + 1e-2> transform->getPosition().y && state == FALLING)
    {
        state = GROUND;
    }
    if(state == GROUND && fabs(_lastGround - transform->getPosition().y) < 1e-1)
    {
        transform->setPosition(
        glm::vec3(transform->getPosition().x, 
        _lastGround, 
        transform->getPosition().z));

    }
    transform->setPosition(
        glm::vec3(transform->getPosition().x, 
        std::max(transform->getPosition().y, _lastGround), 
        transform->getPosition().z));
}

void ThirdPersonCharacterController::handleJump(float dt)
{
    //GUI
    ImGui::Value("x", transform->getPosition().x);
    ImGui::Value("z", transform->getPosition().z);
    
    ImGui::Begin("Jump Physics");
    ImGui::InputFloat("Jump force", &jumpForce);
    ImGui::InputFloat("Jump Time", &jumpTime);
    ImGui::InputFloat("Gravity", &gravity);
    ImGui::Value("Jump Speed", jumpSpeed);
    ImGui::Value("Jump Time", _jumpTime);

    ImGui::End();

    //END GUI
    if (Input::GetButtonDown(BUTTON_0) && state == GROUND)
    {
        LOG(GAME, "STARTING JUMP");
        _curJumpForce = jumpForce;
        _jumpTime = jumpTime;
        state = JUMPING;
    }
    else if (Input::GetButton(BUTTON_0) && state == JUMPING && _jumpTime > 0)
    {


        _jumpTime -= dt;
    }
    else if (state == JUMPING)
    {
        LOG(GAME, "ENDING JUMP");
        state = FALLING;
    }

    //Check if platform under via raycast
    if (state == JUMPING)
    {
        jumpSpeed = _curJumpForce;
    }
    else if (state == FALLING || state == SLIDING)
    {
        jumpSpeed += dt * gravity;
    }
    else
    {
        jumpSpeed = 0;
    }
    if(state != GROUND)
    {
        transform->translate(glm::vec3(0.f, jumpSpeed * dt, 0.f));        
    }

    // if (state == FALLING && GetWorld()->castRay(transform->getPosition(), glm::vec3(0, -1, 0),
    //     bbox->getHalfWidths().y + jumpSpeed * dt, &h))
    // {
    //     //Translate to hit position
    //     transform->translate(h.intersectionPoint - transform->getPosition() +
    //          glm::vec3(0, bbox->getHalfWidths().y, 0));
    //     state = GROUND;
    //     jumpSpeed = 0;
    // }
    // else
    // {
    //     transform->translate(glm::vec3(0.f, jumpSpeed * dt, 0.f));
    // }

    //Check if on ground plane
    if (transform->getPosition().y  <= _lastGround && state == FALLING)
    {
        transform->setPosition(glm::vec3(transform->getPosition().x,
            _lastGround, transform->getPosition().z));
        state = GROUND;
    }

}

void ThirdPersonCharacterController::onCollisionEnter(Collision col)
{
    if ((state == FALLING || state == JUMPING) &&
        glm::dot(col.normal, glm::vec3(0, 1, 0)) > cosf(M_PI / 3) && jumpSpeed < 0)
    {
        LOG(GAME, "Hit ground");
        state = GROUND;
    }
    else if ((state == JUMPING || state == FALLING) &&
        glm::dot(col.normal, glm::vec3(0, -1, 0)) > cosf(M_PI / 3) && jumpSpeed > 0)
    {
        //state = FALLING;
        //jumpSpeed = 0;
    }
}


void ThirdPersonCharacterController::checkIfShouldFall()
{
    //Early break if on ground.
    if (transform->getPosition().y <= _lastGround + 2e-1)
    {
        return;
    }
    if (state != JUMPING && !GetWorld()->castRay(transform->getPosition(),
        glm::vec3(0, -1, 0), 0.1f,nullptr,T_Player))
    {
        state = FALLING;
    }
}

glm::vec3 ThirdPersonCharacterController::checkIfShouldSlide()
{
    if(worldTerrain != nullptr && (state == GROUND || state == SLIDING))
    {
        glm::vec3 pos = gameObject->getTransform().getPosition();
        glm::vec3 normal = worldTerrain->normalAt(pos.x, pos.z);
        LOG(INFO, "Terrain Normal: " + std::to_string(normal.x) + "," + std::to_string(normal.y)  + "," + std::to_string(normal.z));
        const float degToRad70=1.22173;
        if(glm::dot(normal, (glm::vec3(0,1,0))) < cos(degToRad70) && state == GROUND)
        {
            state = SLIDING;

        }
        else if(glm::dot(normal, (glm::vec3(0,1,0))) > cos(degToRad70 + 0.1) && state == SLIDING)
        {
            state = GROUND;
        }
        return normal;

    }
    else
    {
        return glm::vec3(0,1,0);
    }
}


bool ThirdPersonCharacterController::slopeOK(glm::vec3 offset)
{
	if (worldTerrain != nullptr)
	{
		glm::vec3 pos = gameObject->getTransform().getPosition() + offset;
		glm::vec3 normal = worldTerrain->normalAt(pos.x, pos.z);
		float terrainheight = (worldTerrain->heightAt(pos.x, pos.z));
		LOG(INFO, "Terrain Normal: " + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z));
		const float degToRad70 = 1.22173;
		if (glm::dot(normal, (glm::vec3(0, 1, 0))) < cos(degToRad70) && terrainheight > _lastGround)
		{
			return false;

		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

std::shared_ptr<Component> ThirdPersonCharacterController::clone() const
{
    return std::make_shared<ThirdPersonCharacterController>(*this);
}