#include "GameObject.h"
#include "Util/Logger.h"
using namespace MoonEngine;

GameObject::GameObject() :
deleted(false)
{

}

GameObject::GameObject(const Transform & t):
transform(t),
deleted(false)
{

}

GameObject::~GameObject()
{
	LOG(INFO,"Deallocating game object...");
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


void GameObject::addTag(Tag t)
{
	this->tag = t;
}

Tag GameObject::getTag()
{
	return tag;
}
void GameObject::onCollisionEnter(Collision col)
{
	for(Component * c : components)
	{
		c->onCollisionEnter(col);
	}
}
bool GameObject::isDeleted()
{
	return deleted;
}
void GameObject::setDeleted()
{
	deleted = true;
}