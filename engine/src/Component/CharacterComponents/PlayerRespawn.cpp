#include "PlayerRespawn.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

PlayerRespawn::PlayerRespawn():
    lastPos(glm::vec3(0,0,0))
{
}

void PlayerRespawn::respawn()
{
    gameObject->getTransform().setPosition(lastPos);
    GetWorld()->getGameState()->setState(PLAYING_STATE);
}

void PlayerRespawn::start()
{
    lastPos = gameObject->getTransform().getPosition();

    on("picked_up_moon",[&](const Message & msg)
    {
        lastPos = gameObject->getTransform().getPosition();
    });

	on("respawn",[&](const Message & msg)
	{
        respawn();
	});
}

void PlayerRespawn::update(float dt)
{

}

std::shared_ptr<Component> PlayerRespawn::clone() const
{
	return std::make_shared<PlayerRespawn>(*this);
}