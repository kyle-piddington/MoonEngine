#pragma once

#include "Component/Component.h"
/**
 * Controls a character in a first-person matter
 */
namespace MoonEngine
{
    class FirstPersonController: public Component
    {
    public:
        FirstPersonController(float Cam_Move_Speed = 1, float CamSensitivity = 0.002);

        void update(float dt);

        std::shared_ptr<Component> clone() const;

    private:
        float _CamMoveSpeed;
        float _CamSensitivity;
        float _phi;
        float _theta;

    };
}