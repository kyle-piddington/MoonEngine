#include "SunMovement.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Component/Components.h"
using namespace MoonEngine;

SunMovement::SunMovement() :
dirLightObject(nullptr)
{

}

void SunMovement::start()
{
	auto dirLight = GetWorld()->getDirLightObject();
	if(dirLight != nullptr)
	{
		dirLightObject = dirLight->getComponent<DirLight>();
	}
	playerObj = GetWorld()->getPlayer().get();
}

void SunMovement::update(float dt)
{
	if(dirLightObject != nullptr && playerObj != nullptr)
	{
		glm::vec3 sunOffset = playerObj->getTransform().getPosition();
		glm::vec3 dLight = glm::normalize(-dirLightObject->getDirection());
		gameObject->getTransform().setPosition(sunOffset + dLight * 400.0f);
		//Set scale based off of theta. (Later)
		
		
	}
	
	
}

std::shared_ptr<Component> SunMovement::clone() const
{
	return std::make_shared<SunMovement>(*this);
}