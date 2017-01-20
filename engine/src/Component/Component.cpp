#include "Component.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;
Component::Component(std::shared_ptr<GameObject> gameObject)
{
	provideGameObject(gameObject);
}


Component::~Component()
{
	
}

void Component::provideGameObject(std::shared_ptr<GameObject> object)
{
	this->gameObject = object;
	object->addComponent(this);
}

void Component::update(float dt)
{

}

void Component::onCollisionEnter(Collision col)
{

}

void Component::onCollisionExit(Collision col)
{

}
