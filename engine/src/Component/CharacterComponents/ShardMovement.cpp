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
	startingTime = rand();
}

void ShardMovement::update(float dt)
{
	gameObject->getTransform().rotate(glm::vec3(0, dt, 0));

	startingTime += dt;
	
	gameObject->getTransform().translate(glm::vec3(0,  0.01 * sinf(startingTime), 0));
}

std::shared_ptr<Component> ShardMovement::clone() const
{
	return std::make_shared<ShardMovement>(*this);
}