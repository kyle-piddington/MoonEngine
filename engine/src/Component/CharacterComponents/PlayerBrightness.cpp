#include <thirdparty/imgui/imgui.h>
#include "PlayerBrightness.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Component/LightComponents/PointLight.h"
#include "glm/gtc/type_ptr.hpp"

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

	_linear = _pointLight->getAttenuation().y;
	_exp = _pointLight->getAttenuation().z;

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
	ImGui::Begin("Bright");
	ImGui::DragFloat("Linear", &_linear);
	ImGui::DragFloat("Exp", &_exp);
    ImGui::DragFloat3("Color", glm::value_ptr(color));
	ImGui::End();

	_pointLight->setAttenuation(_linear, _exp);
    _pointLight->setColor(color);
}

std::shared_ptr<Component> PlayerBrightness::clone() const
{
	return std::make_shared<PlayerBrightness>(*this);
}

glm::vec3 PlayerBrightness::getColor()
{
	return color;
}

float PlayerBrightness::getDistance()
{
	return distance;
}