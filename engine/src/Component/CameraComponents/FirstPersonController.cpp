#include "FirstPersonController.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "GameObject/GameObject.h"
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
   if(Mouse::pressed(GLFW_MOUSE_BUTTON_LEFT))
   {
         rotate.y = (Mouse::getLastY() - Mouse::getY())* _CamSensitivity;
         rotate.x = (Mouse::getLastX() - Mouse::getX())* _CamSensitivity;
         _phi += rotate.y;
         _theta += rotate.x;
         rotate.z = 0;
   }
   glm::vec2 translateVec;
   if(Keyboard::isKeyDown(GLFW_KEY_W))
   {
      translateVec.y += _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_S))
   {
      translateVec.y -= _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_A))
   {
      translateVec.x += _CamMoveSpeed * dt;
   }
   if(Keyboard::isKeyDown(GLFW_KEY_D))
   {
      translateVec.x -= _CamMoveSpeed * dt;
   }

   Transform & transform = gameObject->getTransform();

   transform.setRotation(glm::vec3(_phi,_theta,0.0));
   transform.translate(transform.forward() * translateVec.y + transform.right() * translateVec.x);
}