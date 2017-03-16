#pragma once

#include <Component/Component.h>
#include <vector>
#include "glm/glm.hpp"

namespace MoonEngine
{
    struct CameraStep {
        glm::vec3 cameraPos;
        glm::vec3 lookAtPos;
    };

    class CameraCutscene: public Component
    {
    public:
        CameraCutscene();

        void start();
        void update(float dt);

        std::shared_ptr<Component> clone() const;

        void setSteps(std::vector<CameraStep> steps);
        void loadSteps(std::string file);
        void setStepPlayer(bool start, bool end);
        void setNextState(string state);

    private:
        bool _running;
        int _currentStep;

        glm::vec3 _fromPos;
        glm::vec3 _toPos;
        glm::vec3 _fromLookAt;
        glm::vec3 _toLookAt;
        glm::vec3 _playerPos;

        float _maxStepAmount;
        float _currentStepRatio;
        std::vector<CameraStep> _cameraSteps;

        void fetchCurrentSteps();

        bool _startPlayer;
        bool _endPlayer;

        string _nextState;

    };

}