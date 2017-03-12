#include <Util/Logger.h>
#include <Component/CameraComponents/Camera.h>
#include <MoonEngine.h>
#include "GameState.h"

using namespace MoonEngine;

GameState::GameState():
_currentState(MENU_STATE)
{

}

void GameState::start()
{
    on(INTRO_STATE, [&](const Message & msg) {
        LOG(INFO, "Adding cutscene");
        GameObject * cameraObj = GetWorld()->findGameObjectWithComponent<Camera>();
        Component * cutscene = GetWorld()->createComponent<CameraCutscene>();
        cutscene->start();
        cameraObj->addComponent(cutscene);
    });
}

void GameState::update(float dt)
{
}

void GameState::setState(std::string state)
{
    _currentState = state;
    sendGlobalMessage(state);
}

std::shared_ptr<Component> GameState::clone() const
{
    return std::make_shared<GameState>(*this);
}