#include "PlayerTimer.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "IO/Keyboard.h"

using namespace MoonEngine;

PlayerTimer::PlayerTimer():
    _times({
        0.0f,
        0.33f,
        0.66f,
        1.0f
	}),
    _currentTime(1),
    _timing(true)
{
}

void PlayerTimer::start()
{
    on("picked_up_moon",[&](const Message & msg)
    {
        _currentTime++;
    });

    on("respawn",[&](const Message & msg)
    {
        GetWorld()->setGlobalTime(_times[_currentTime - 1]);
        _timing = true;
    });
}

void PlayerTimer::update(float dt)
{
    if (!_timing || _currentTime >= _times.size()) return;
    if (GetWorld()->getGlobalTime() > _times[_currentTime])
    {
        _timing = false;
        Keyboard::reset();
        GetWorld()->getGameState()->setState(RESPAWNING_STATE);
        sendGlobalMessage("out_of_time");
    }

}

std::shared_ptr<Component> PlayerTimer::clone() const
{
	return std::make_shared<PlayerTimer>(*this);
}