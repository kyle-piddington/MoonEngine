#include <iostream>
#include <Geometry/Transform.h>
#include <MoonEngine.h>

using namespace MoonEngine;

CameraCutscene::CameraCutscene():
    _running(true),
    _currentStep(0),
    _maxStepAmount(1.0),
    _startPlayer(false),
    _endPlayer(false)
{
}

void CameraCutscene::loadSteps(string file)
{
    setSteps(_cameraSteps);
}

void CameraCutscene::setStepPlayer(bool start, bool end)
{
    _startPlayer = start;
    _endPlayer = end;
}

void CameraCutscene::setSteps(std::vector<CameraStep> steps)
{
    _cameraSteps = steps;
    if (_startPlayer)
    {
        _cameraSteps.insert(_cameraSteps.begin(), {_playerPos + glm::vec3(5,5,5), _playerPos});
    }
    if (_endPlayer)
    {
        _cameraSteps.push_back({_playerPos + glm::vec3(5,5,5), _playerPos});
    }
}

void CameraCutscene::start()
{
    // cameraPos, lookAtPos
    std::vector<CameraStep> steps;
    steps.push_back({glm::vec3(0, 200, 0), glm::vec3(-32.62, 20.91, -101.99)});
    steps.push_back({glm::vec3(100, 200, 0), glm::vec3(-32.62, 20.91, -101.99)});
    steps.push_back({glm::vec3(100, 200, 100), glm::vec3(-32.62, 20.91, -101.99)});
    steps.push_back({glm::vec3(0, 200, 100), glm::vec3(100, 100.91, -101.99)});
    setStepPlayer(false, true);
    setSteps(steps);

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
    if (!_running) {
        return;
    }
    if (_currentStep >= _cameraSteps.size() - 1) {
        _running = false;
        GetWorld()->getGameState()->setState(PLAYING_STATE);
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