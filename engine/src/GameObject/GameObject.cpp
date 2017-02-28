#include "GameObject.h"
#include "Util/Logger.h"
#include "Component/Components.h"

using namespace MoonEngine;

GameObject::GameObject():
deleted(false),
defaultBox(glm::vec3(0,0,0),0.5f,0.5f,0.5f),
useMeshBounds(false),
useBoxColliderBounds(false),
tag(T_NONE)
{

}

GameObject::GameObject(const Transform & t):
transform(t),
deleted(false),
useMeshBounds(false),
useBoxColliderBounds(false),
tag(T_NONE)
{

}

GameObject::~GameObject()
{
    LOG(INFO, "Deallocating game object...");
}

void GameObject::start()
{

    for (Component * c : components)
    {
        c->start();
    }
}


void GameObject::addComponent(Component * component)
{
    component->provideGameObject(this);
    if(dynamic_cast<Mesh *>(component) != nullptr)
    {
        useMeshBounds = true;
    }
    // else if(dynamic_cast<BoxCollider *>(component) != nullptr)
    // {
    //     useBoxColliderBounds = true;
    // }
    components.push_back(component);
}

void GameObject::addHandler(std::string message, const messageFn & fn)
{
    if(messageHandlers.find(message) != messageHandlers.end())
    {
        messageHandlers[message] = std::vector<messageFn>();
    }
    messageHandlers[message].push_back(fn);
}

void GameObject::addMessage(Message msg)
{
    waitingMessages.push_back(msg);
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
    defaultTransformedBox = defaultBox.transform(transform.getMatrix());
    //For each waiting message
    for(const Message & msg : waitingMessages)
    {
        if(messageHandlers.find(msg.message) != messageHandlers.end())
        {
            //If a handler for the message exists
            for(auto handler : messageHandlers[msg.message])
            {
                //call the handler
                handler(msg);
            }
        }
    }
    //Clear the waiting messages
    waitingMessages.clear();
    //Update all components
    for (Component * c : components)
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
    for (Component * c : components)
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

const BoundingBox & GameObject::getBounds()
{
    if(isDeleted())
    {
        return defaultTransformedBox;
    }
    if(useBoxColliderBounds)
    {
        return (getComponent<BoxCollider>()->getBoundingBox());
    }
    else if(useMeshBounds)
    {
        return (getComponent<Mesh>()->getBoundingBox());
    }
    else
    {
        return defaultTransformedBox;
    }
}

void GameObject::addNode(Node *node)
{
	region.push_back(node);
}

void GameObject::removeNode(Node *node)
{
	for (int i = 0; i < region.size(); i++)
	{
		if (node == region.at(i))
		{
			region.erase(region.begin() + i);
			i--;
		}
	}
}

std::vector<Node *> GameObject::getNodes()
{
	return region;
}