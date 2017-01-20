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

void GameObject::addComponent(Component * component)
{
	components.push_back(component);
}

const Transform & GameObject::getTransform() const
{
	return transform;
}	

Transform & GameObject::getTransform()
{
	return transform;
}