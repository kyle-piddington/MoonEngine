#include "GameObject.h"
using namespace MoonEngine;

GameObject::GameObject()
{

}

GameObject::GameObject(const Transform & t):
transform(t)
{

}

GameObject::~GameObject()
{

}

void GameObject::start(){

	for(Component * c : components)
	{
		c->start();
	}
}

void GameObject::addComponent(Component * component)
{
	component->provideGameObject(this);
	components.push_back(component);
}


Transform & GameObject::getTransform()
{
	return transform;
}

std::vector<Component *> GameObject::getComponents()
{
	return components;
}

void GameObject::update(float dt)
{
	for(Component * c : components)
	{
		c->update(dt);
	}
}

void GameObject::onCollisionEnter(Collision col)
{
	for(Component * c : components)
	{
		c->onCollisionEnter(col);
	}
}