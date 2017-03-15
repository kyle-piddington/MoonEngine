#include <iostream>
#include <Geometry/Transform.h>
#include <MoonEngine.h>
#include <IO/TextLoader.h>

using namespace MoonEngine;

CameraCutscene::CameraCutscene():
    _running(false),
    _currentStep(0),
    _maxStepAmount(1.0),
    _startPlayer(false),
    _endPlayer(false)
{
}

void CameraCutscene::loadSteps(string file)
{
    std::vector<CameraStep> newSteps;

    std::string levelInfo = TextLoader::LoadFullFile(Library::getResourcePath() + file);

    //Try and open the file first
    //Parse JSON
    rapidjson::Document document;
    rapidjson::ParseResult pr = document.Parse(levelInfo.c_str());
    if (!pr)
    {
        LOG(ERROR, "LEVEL: Could not parse document json. ( " + std::string(rapidjson::GetParseError_En(pr.Code())) +
                   " [" + std::to_string(pr.Offset()) + "] )");
        return;
    }

    const rapidjson::Value & steps = document["steps"];

    /* Load all the steps */
    for (auto & step : steps.GetArray())
    {
        const rapidjson::Value & rawPos = step[0];
        glm::vec3 stepPos = glm::vec3(rawPos[0].GetFloat(), rawPos[1].GetFloat(), rawPos[2].GetFloat());

        const rapidjson::Value & rawLookAt = step[1];
        glm::vec3 stepLookAt = glm::vec3(rawLookAt[0].GetFloat(), rawLookAt[1].GetFloat(), rawLookAt[2].GetFloat());

        newSteps.push_back({stepPos, stepLookAt});
    }

    setSteps(newSteps);
}

void CameraCutscene::setStepPlayer(bool start, bool end)
{
    _startPlayer = start;
    _endPlayer = end;
}

void CameraCutscene::setSteps(std::vector<CameraStep> steps)
{
    _cameraSteps = steps;

    _playerPos = GetWorld()->getPlayer()->getTransform().getPosition();
    if (_startPlayer)
    {
        _cameraSteps.insert(_cameraSteps.begin(), {_playerPos + glm::vec3(5,5,5), _playerPos});
    }
    if (_endPlayer)
    {
        _cameraSteps.push_back({_playerPos + glm::vec3(5,5,5), _playerPos});
    }

    fetchCurrentSteps();
}

void CameraCutscene::start()
{
}

void CameraCutscene::fetchCurrentSteps() {
    _fromPos = _cameraSteps[_currentStep].cameraPos;
    _toPos = _cameraSteps[_currentStep + 1].cameraPos;

    _fromLookAt = _cameraSteps[_currentStep].lookAtPos;
    _toLookAt = _cameraSteps[_currentStep + 1].lookAtPos;

    // Keep position and lookat point in sync
    _currentStepRatio = distance(_fromPos, _toPos) / distance(_fromLookAt, _toLookAt);
    _running = true;
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

    if (Keyboard::key(GLFW_KEY_SPACE)) {
        _maxStepAmount *= 10;
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