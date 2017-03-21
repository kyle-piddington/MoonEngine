#include "CollectableComponent.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

CollectableComponent::CollectableComponent(std::string name):
    _name(name),
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

		int count = _name == "shard" ? 7 : 100;
		for (int i = 0; i < count; i++)
		{
			GetWorld()->instantiate(GetWorld()->getPrefab(_name + "Particle").get(), gameObject->getTransform());
		}
		sendGlobalMessage("picked_up_moon");//"picked_up_" + _name);
        _collected = true;
		Delete(gameObject);

    }
}

std::shared_ptr<Component> CollectableComponent::clone() const
{
    return std::make_shared<CollectableComponent>(*this);
}