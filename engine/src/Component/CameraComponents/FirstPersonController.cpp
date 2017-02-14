#include "FirstPersonController.h"
#include "IO/Mouse.h"
#include "IO/Input.h"
#include "GlobalFuncs/GlobalFuncs.h"

//Windows C++
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

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
    if (!isImguiEnabled())
    {
        glm::vec3 rotate(0.0);

        rotate.y = (Mouse::getLastY() - Mouse::getY()) * _CamSensitivity;
        rotate.x = (Mouse::getLastX() - Mouse::getX()) * _CamSensitivity;
        rotate.y += Input::GetAxis(AXIS_VERTICAL_1) * _CamMoveSpeed * dt;
        rotate.x += Input::GetAxis(AXIS_HORIZONTAL_1) * _CamMoveSpeed * dt;

        _phi += rotate.y;
        _theta += rotate.x;
        _phi = std::min(_phi, (float) M_PI / 2.0f - 0.1f);
        _phi = std::max(_phi, -(float) M_PI / 2.0f + 0.1f);
    }

    glm::vec2 translateVec;
    translateVec.x = -Input::GetAxis(AXIS_HORIZONTAL_0);
    translateVec.y = Input::GetAxis(AXIS_VERTICAL_0);
    translateVec *= _CamMoveSpeed * dt;


    Transform & transform = gameObject->getTransform();
    transform.setRotation(glm::vec3(_phi, _theta, 0.0));
    transform.translate(transform.forward() * translateVec.y + transform.right() * translateVec.x);
}

std::shared_ptr<Component> FirstPersonController::clone() const
{
    return std::make_shared<FirstPersonController>(*this);
}