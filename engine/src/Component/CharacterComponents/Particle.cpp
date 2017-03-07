#include "Particle.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include <iostream>

using namespace MoonEngine;

Particle::Particle() :
	accumTime(0),
	state(INIT)
{

}

void Particle::start()
{
	float a = (float)rand() / RAND_MAX * 6.28f;
	float b = (float)rand() / RAND_MAX * 6.28f;
	float c = (float)rand() / RAND_MAX * 6.28f;
	direction = glm::normalize(glm::vec3(a, b, c));
	player = GetWorld()->getPlayer();
}

void Particle::update(float dt)
{
	accumTime += dt;
	if (state == INIT)
	{
		gameObject->getTransform().translate(dt * direction);
		if (accumTime >= 0.7f)
		{
			state = GATHER;
		}
	}
	else
	{
		if (player != nullptr)
		{
			direction = glm::normalize(player->getTransform().getPosition()
				- gameObject->getTransform().getPosition());
		}
		
		float scale = glm::length(player->getTransform().getPosition() - gameObject->getTransform().getPosition());

		gameObject->getTransform().translate(direction / 6.0f);
		gameObject->getTransform().setScale(std::min(0.25f, scale));
		if (scale <= 0.1f)
		{
			Delete(gameObject);
		}
	}
	if (accumTime >= 1.0)
	{
		Delete(gameObject);
	}
}

std::shared_ptr<Component> Particle::clone() const
{
	return std::make_shared<Particle>(*this);
}
