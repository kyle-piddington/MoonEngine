#pragma once

#include <Component/Component.h>
#include <vector>
#include "glm/glm.hpp"

namespace MoonEngine
{
    class CameraRecorder: public Component
    {
    public:
        CameraRecorder();

        void start();
        void update(float dt);

        std::shared_ptr<Component> clone() const;

    private:
        bool _recording;

        std::vector<CameraStep> _recordedSteps;

        void saveSteps();

    };

}