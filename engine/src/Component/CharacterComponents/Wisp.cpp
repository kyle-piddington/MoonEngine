#include "Wisp.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include <iostream>

using namespace MoonEngine;

Wisp::Wisp() :
	accumTime(0),
	scale(1)
{

}

void Wisp::start()
{
	player = GetWorld()->getPlayer();
	restart();

}

void Wisp::restart()
{
	pBox = player->getBounds();
	int x = (int)rand() / RAND_MAX * 6.28f - 3.14;
	float xPos, yPos, zPos;
	if (x % 2)
	{
		xPos = pBox.max().x;
	}
	else
	{
		xPos = pBox.min().x;
	}
	yPos = pBox.min().x;

	float r = ((float)rand() / RAND_MAX) + 0.00001f;
	zPos = pBox.min().z + r * (pBox.max().z - pBox.min().z);
	gameObject->getComponent<Transform>()->setPosition(glm::vec3(xPos, yPos, zPos));
	gameObject->getTransform().setRotation(player->getTransform().getRotation());
	gameObject->getTransform().setScale(scale);
	accumTime = 0.0f;
}

void Wisp::update(float dt)
{
	
	accumTime += dt;
	scale *= 0.5 * dt;
	gameObject->getTransform().setScale(std::min(0.1f, scale));
	if (accumTime <= 0.2 || accumTime > 0.4 && accumTime <= 0.6 || accumTime > 0.8)
	{
		gameObject->getComponent<Transform>()->translate(glm::vec3(0, dt, dt));
	}
	else
	{
		gameObject->getComponent<Transform>()->translate(glm::vec3(0, dt, -dt));
	}
	if (accumTime > 1)
	{
		restart();
	}
}

std::shared_ptr<Component> Wisp::clone() const
{
	return std::make_shared<Wisp>(*this);
}