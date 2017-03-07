#include "Particle.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

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

		if (accumTime >= 0.5f)
		{
			state = GATHER;
		}
	}
	else if (state == GATHER)
	{
		
		if (player != nullptr)
		{
			direction = dt * glm::normalize(gameObject->getTransform().getPosition() - player->getTransform().getPosition());
		}
		gameObject->getTransform().translate(dt * direction);
	}
	else
	{
		if (accumTime >= 2.5)
		{
			Delete(gameObject);
		}
	}
	if (state != END && player->getBounds().contains(gameObject->getBounds()))
	{
		state = END;
	}
}

std::shared_ptr<Component> Particle::clone() const
{
	return std::make_shared<Particle>(*this);
}
