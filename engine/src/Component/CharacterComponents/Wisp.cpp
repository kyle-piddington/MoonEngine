#include "Wisp.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include <iostream>

using namespace MoonEngine;

Wisp::Wisp() :
	accumTime(1.0f),
	scale(0.0f)
{

}

void Wisp::start()
{
	player = GetWorld()->getPlayer();

}

void Wisp::restart()
{
	pBox = player->getBounds();
	int x = (int)rand();// / RAND_MAX * 6.28f - 3.14f;
	float xPos, yPos, zPos;
	if (x % 2)
	{
		xPos = pBox.max().x;
		//xPos = player->getTransform().getPosition().x + 0.1;
	}
	else
	{
		xPos = pBox.min().x;
		//xPos = player->getTransform().getPosition().x - 0.2;
	}
	yPos = pBox.min().y;

	float r = ((float)rand() / RAND_MAX) + 0.00001f;
	zPos = pBox.min().z + 2 * (r * (pBox.max().z - pBox.min().z)) - (pBox.max().z - pBox.min().z);
	gameObject->getTransform().setPosition(glm::vec3(xPos, yPos, zPos));
	gameObject->getTransform().setRotation(player->getTransform().getRotation());
	gameObject->getTransform().setScale(0.0f);
	accumTime = 0.0f;
}

void Wisp::update(float dt)
{
	
	accumTime += dt;
	scale = 0.02f;//1.0f / (8.0f * dt);//(2.0f - dt)/ 4.0f;
	gameObject->getTransform().setScale(scale);
	if (accumTime <= 0.2 || accumTime > 0.4 && accumTime <= 0.6 || accumTime > 0.8)
	{
		gameObject->getTransform().translate(glm::vec3(0, dt, dt));
	}
	else
	{
		gameObject->getTransform().translate(glm::vec3(0, dt, -dt));
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