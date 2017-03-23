#include "MoonSpriteComponent.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "util/MathUtil.h"
#include "Component/Components.h"
using namespace MoonEngine;

MoonSpriteComponent::MoonSpriteComponent() :
	dirLightObject(nullptr)
{

}

void MoonSpriteComponent::start()
{
	auto dirLight = GetWorld()->getDirLightObject();
	if (dirLight != nullptr)
	{
		dirLightObject = dirLight->getComponent<DirLight>();
		dirLightObject->setColor(glm::vec3(0.3, 0.3, 0.6));
	}
	playerObj = GetWorld()->getPlayer().get();
}

void MoonSpriteComponent::update(float dt)
{

		_raiseTime += dt/10.f;
		gameObject->getTransform().setPosition(glm::vec3(551, MathUtil::clamp(_raiseTime, 0, 1) * 150.f, 105));
		dirLightObject->setDirection(GetWorld()->getPlayer()->getTransform().getPosition() - gameObject->getTransform().getPosition());
		//Set scale based off of theta. (Later)


}

std::shared_ptr<Component> MoonSpriteComponent::clone() const
{
	return std::make_shared<MoonSpriteComponent>(*this);
}