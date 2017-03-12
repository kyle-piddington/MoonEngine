#include <iostream>
#include <Geometry/Transform.h>
#include "CameraCutscene.h"
#include "Util/MathUtil.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;

CameraCutscene::CameraCutscene():
    _currentStep(0),
    _maxStepAmount(1.0)
{

}

void CameraCutscene::start()
{
    // cameraPos, lookAtPos
    _cameraSteps.push_back({glm::vec3(0, 200, 0), glm::vec3(-32.62, 20.91, -101.99)});
    _cameraSteps.push_back({glm::vec3(-32.62, 150.91, -101.99), glm::vec3(-32.62, 20.91, -101.99)});
    _cameraSteps.push_back({glm::vec3(-32.62, 25.91, -101.99), glm::vec3(-32.62, 20.91, -101.99)});

    fetchCurrentSteps();
}

void CameraCutscene::fetchCurrentSteps() {
    _fromPos = _cameraSteps[_currentStep].cameraPos;
    _toPos = _cameraSteps[_currentStep + 1].cameraPos;

    _fromLookAt = _cameraSteps[_currentStep].lookAtPos;
    _toLookAt = _cameraSteps[_currentStep + 1].lookAtPos;

    // Keep position and lookat point in sync
    _currentStepRatio = distance(_fromPos, _toPos) / distance(_fromLookAt, _toLookAt);
}

void CameraCutscene::update(float dt)
{
    if (_currentStep >= _cameraSteps.size() - 1) {
        return;
    }

    // Interpolate
    _fromPos = MathUtil::moveTowards(_fromPos, _toPos, _maxStepAmount);
    _fromLookAt = MathUtil::moveTowards(_fromLookAt, _toLookAt, _maxStepAmount / _currentStepRatio);

    // Get next point if done
    if (_fromPos == _toPos) {
        _currentStep++;
        fetchCurrentSteps();
    }

    Transform & transform = gameObject->getTransform();
    transform.setPosition(_fromPos);
    transform.lookAt(_fromLookAt);
}

std::shared_ptr<Component> CameraCutscene::clone() const
{
    return std::make_shared<CameraCutscene>(*this);
}