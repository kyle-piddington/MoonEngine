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
	BoundingBox box = player->getBounds();
	int x = (int)rand() / RAND_MAX * 6.28f - 3.14;
	float xPos, yPos, zPos;
	if (x % 2)
	{
		xPos = box.max().x;
	}
	else
	{
		xPos = box.min().x;
	}
	yPos = box.min().x;

	float r = ((float)rand() / RAND_MAX) + 0.00001f;
	zPos = box.min().z + r * (box.max().z - box.min().z);
	gameObject->getComponent<Transform>()->setPosition(glm::vec3(xPos, yPos, zPos));
	gameObject->getTransform().setRotation(player->getTransform().getRotation());

}

void Wisp::update(float dt)
{
	
	accumTime += dt;
	scale -= 0.5 * dt;
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
		Delete(gameObject);
	}
}

std::shared_ptr<Component> Wisp::clone() const
{
	return std::make_shared<Wisp>(*this);
}