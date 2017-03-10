#include <Util/Logger.h>
#include "GameState.h"

using namespace MoonEngine;

GameState::GameState():
_currentState(MENU_STATE)
{

}

void GameState::start()
{
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