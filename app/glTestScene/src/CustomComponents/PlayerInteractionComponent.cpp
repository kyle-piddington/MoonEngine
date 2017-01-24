#include "MoonEngine.h"
#include "PlayerInteractionComponent.h"
#include "CharacterMoveComponent.h"

using namespace MoonEngine;

void PlayerInteractionComponent::onCollisionEnter(Collision col)
{
	CharacterMoveComponent * comp = col.other->getComponent<CharacterMoveComponent>();
	if(comp != nullptr && !comp->collidingWithPlayer())
	{
		comp->playerCollided();
		_numCollisions++;
	}
}

std::shared_ptr<Component> PlayerInteractionComponent::clone() const
{
	return std::make_shared<PlayerInteractionComponent>(*this);
}

