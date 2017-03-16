#include "PlayerBrightness.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

PlayerBrightness::PlayerBrightness() :
	distance(3.0),
    color(glm::vec3(0.0, 1.0, 1.0))
{

}

void PlayerBrightness::increaseBrightness(glm::vec3 colorIncrease, float distanceIncrease)
{
    distance += distanceIncrease;
    _pointLight->setRange(distance);

    color += colorIncrease;
    _pointLight->setColor(color);
}

void PlayerBrightness::start()
{
    _pointLight = gameObject->getComponent<PointLight>();

	on("picked_up_shard",[&](const Message & msg)
	{
        increaseBrightness(glm::vec3(0.0, 1.0, 1.0), 0.2);
	});

	on("picked_up_moon",[&](const Message & msg)
	{
        increaseBrightness(glm::vec3(0.0, 2.0, 2.0), 4.0);
	});
}

void PlayerBrightness::update(float dt)
{

}

std::shared_ptr<Component> PlayerBrightness::clone() const
{
	return std::make_shared<PlayerBrightness>(*this);
}