#include "CollectableComponent.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

CollectableComponent::CollectableComponent(std::string eventName):
    _eventName(eventName),
    _collected(false)
{

}

void CollectableComponent::start()
{
	if (gameObject->getComponent<BoxCollider>() != nullptr)
	{
		gameObject->getComponent<BoxCollider>()->isTrigger = true;
	}
}

void CollectableComponent::update(float dt)
{

}

void CollectableComponent::onCollisionEnter(Collision col)
{
    if (_collected) {
        return;
    }
    if (T_Player == col.other->getTag())
    {
    	sendMessage("collected");
		sendGlobalMessage(_eventName);
        _collected = true;
    }
}

std::shared_ptr<Component> CollectableComponent::clone() const
{
    return std::make_shared<CollectableComponent>(*this);
}