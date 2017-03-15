#include "BeamComponent.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Util/MathUtil.h"

using namespace MoonEngine;

BeamComponent::BeamComponent() 
{

}

void BeamComponent::start()
{
	playerObject = GetWorld()->getPlayer().get();
	xzPos = glm::vec2(gameObject->getTransform().getPosition().x, gameObject->getTransform().getPosition().z); 
}

void BeamComponent::update(float dt)
{
	glm::vec2 playerXZ = glm::vec2(playerObject->getTransform().getPosition().x, playerObject->getTransform().getPosition().z); 
	float length = glm::length(playerXZ - xzPos);
	float thickness = std::min(1.0f,std::max(0.0f,MathUtil::lerp(0,1,length/10.0f)));
	gameObject->getTransform().setScale(glm::vec3(thickness,gameObject->getTransform().getScale().y,thickness));
}

std::shared_ptr<Component> BeamComponent::clone() const
{
	return std::make_shared<BeamComponent>(*this);
}