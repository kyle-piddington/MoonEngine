#include "Component.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;
Component::Component()
{
	
}


Component::~Component()
{
	
}

void Component::start()
{

}
void Component::provideGameObject(GameObject * object)
{
	this->gameObject = object;
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
