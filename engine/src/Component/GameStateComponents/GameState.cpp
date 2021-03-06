#include <Util/Logger.h>
#include <Component/CameraComponents/Camera.h>
#include <MoonEngine.h>
#include "GameState.h"

using namespace MoonEngine;

GameState::GameState(std::string state):
_currentState(state)
{

}

void GameState::start()
{
    on(INTRO_STATE, [&](const Message & msg)
    {
        LOG(ERROR, "Adding cutscene");
        AudioService::GetAudio()->playSound("bgMusic.mp3");
        AudioService::GetAudio()->playSound("windgrass1.mp3");

        GameObject * cameraObj = GetWorld()->findGameObjectWithComponent<Camera>();
        CameraCutscene * cutscene = GetWorld()->createComponent<CameraCutscene>();
        cutscene->setStepPlayer(false, true);
        cutscene->loadSteps("cutscene.json");
        cutscene->start();
        cameraObj->addComponent(cutscene);
        //GetWorld()->getGameState()->setState(PLAYING_STATE);
    });

    on(PLAYING_STATE, [&](const Message & msg)
    {
        LOG(INFO, "Adding orbital camera");
        GameObject * cameraObj = GetWorld()->findGameObjectWithComponent<Camera>();
        if (cameraObj->getComponent<FirstPersonController>() == nullptr)
        {
            Component * control = GetWorld()->createComponent<ThirdPersonOrbitalController>();
            cameraObj->addComponent(control);
            control->start();
        }
    });

    on(ENDING_STATE, [&](const Message & msg)
    {
        LOG(INFO, "Adding cutscene");
        GameObject * cameraObj = GetWorld()->findGameObjectWithComponent<Camera>();
        cameraObj->getComponent<ThirdPersonOrbitalController>()->setDisabled();
		Instantiate(GetWorld()->getPrefab("moonBillboard").get());
		GetWorld()->setGlobalTime(0.95f);
       CameraCutscene * cutscene = cameraObj->getComponent<CameraCutscene>();
        cutscene->setStepPlayer(true, false);
        cutscene->setNextState(ENDED_STATE);
        cutscene->loadSteps("endscene.json");
        cutscene->start();
    });

}

void GameState::update(float dt)
{
}

void GameState::setState(std::string state)
{
    LOG(ERROR, "NEW STATE: " + state);
    _currentState = state;
    sendGlobalMessage(state);
}

std::shared_ptr<Component> GameState::clone() const
{
    return std::make_shared<GameState>(*this);
}