#include "FirstPersonController.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "GameObject/GameObject.h"
//Windows C++
#ifndef M_PI
#define M_PI 3.141592653589793
#endif
#include <cmath>
#include <algorithm>

using namespace MoonEngine;
FirstPersonController::FirstPersonController(float Cam_Move_Speed, float CamSensitivity):
_CamMoveSpeed(Cam_Move_Speed),
_CamSensitivity(CamSensitivity),
_phi(0),
_theta(0)
{

}

void FirstPersonController::update(float dt)
{
	glm::vec3 rotate(0.0);

   rotate.y = (Mouse::getLastY() - Mouse::getY())* _CamSensitivity;
   rotate.x = (Mouse::getLastX() - Mouse::getX())* _CamSensitivity;
   _phi += rotate.y;
   _theta += rotate.x;
   _phi = std::min(_phi, (float)M_PI/2.0f - 0.1f);
   _phi = std::max(_phi, -(float)M_PI/2.0f + 0.1f);
   
   rotate.z = 0;
   
   glm::vec2 translateVec;
   if(Keyboard::isKeyDown(GLFW_KEY_W))
   {
      translateVec.y -= _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_S))
   {
      translateVec.y += _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_A))
   {
      translateVec.x -= _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_D))
   {
      translateVec.x += _CamMoveSpeed * dt;
   }

   Transform & transform = gameObject->getTransform();
   transform.setRotation(glm::vec3(_phi,_theta,0.0));
   transform.translate(transform.forward() * translateVec.y + transform.right() * translateVec.x);
   //Terrible hardcoding for Lab1 (I'm sorry =( )
   if(transform.getPosition().y < 0.2)
   {
      transform.translate(glm::vec3(0,0.2 - transform.getPosition().y ,0));
   }
}

std::shared_ptr<Component> FirstPersonController::clone() const
{
   return std::make_shared<FirstPersonController>(*this);
}