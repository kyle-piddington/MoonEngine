#include "ShardMovement.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

ShardMovement::ShardMovement() :
	startingTime(0)
{

}

void ShardMovement::start()
{
	startingTime = (float)rand()/RAND_MAX * 6.28f;
	gameObject->getTransform().rotate(glm::vec3(0,startingTime,0));
	gameObject->getTransform().setScale(glm::vec3(0.25,0.25,0.25));
}

void ShardMovement::update(float dt)
{
	gameObject->getTransform().rotate(glm::vec3(0, dt, 0));
	startingTime += dt;
	gameObject->getTransform().translate(glm::vec3(0,  0.0025 * sinf(startingTime), 0));
}

std::shared_ptr<Component> ShardMovement::clone() const
{
	return std::make_shared<ShardMovement>(*this);
}