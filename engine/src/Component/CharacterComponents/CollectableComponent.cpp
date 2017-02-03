#include "CollectableComponent.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

CollectableComponent::CollectableComponent()
{

}

void CollectableComponent::start()
{

}

void CollectableComponent::update(float dt)
{

}

void CollectableComponent::onCollisionEnter(Collision col)
{
    if (T_Player == col.other->getTag())
    {
        Delete(gameObject);
    }
}

std::shared_ptr<Component> CollectableComponent::clone() const
{
    return std::make_shared<CollectableComponent>(*this);
}