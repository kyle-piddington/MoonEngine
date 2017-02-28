#include "Component.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
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

void Component::on(std::string message, const messageFn & fn)
{
	gameObject->addHandler(message, fn);
}

void Component::sendMessage(std::string messageStr)
{
	Message msg;
	msg.sendingObject = gameObject;
	msg.sendingComponent = this;
	msg.message = messageStr;
	gameObject->addMessage(msg);
}

void Component::sendGlobalMessage(std::string messageStr)
{
	Message msg;
	msg.sendingObject = gameObject;
	msg.sendingComponent = this;
	msg.message = messageStr;
	GetWorld()->addGlobalMessage(msg);
}


bool Component::isDeleted()
{
    return deleted;
}

void Component::setDeleted()
{
    deleted = true;
}