#include "Component.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;

Component::Component():
    deleted(false)
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

bool Component::isDeleted()
{
    return deleted;
}

void Component::setDeleted()
{
    deleted = true;
}