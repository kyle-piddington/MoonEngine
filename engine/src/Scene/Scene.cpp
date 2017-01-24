#include "Scene.h"
#include "Component/Components.h"
#include "Util/Logger.h"
using namespace MoonEngine;


Scene::Scene()
{
	_allGameObjects.clear();
	_gameObjects.clear();
	_renderableGameObjects.clear();
	_boxCollisionComponents.clear();
	_components.clear();
}

/**
 * Add the gameobject to the active scene.
 * @param obj [description]
 */
void Scene::addGameObject(std::shared_ptr<GameObject> obj)
{
	_gameObjects.push_back(obj);
	if(obj->getComponent<Material>() != nullptr && 
		obj->getComponent<Mesh>() != nullptr)
	{
		LOG(INFO, "Adding renderable game object");
		_renderableGameObjects.push_back(obj);
	}
	BoxCollider * col = obj->getComponent<BoxCollider>();
	if(col != nullptr)
	{
		LOG(INFO, "Adding collidable game object");
		_boxCollisionComponents.push_back(col);
	}
}

void Scene::runUpdate(float dt)
{
	instantiateNewObjects();
	for(std::shared_ptr<GameObject> go :  _gameObjects)
	{
		go->update(dt);
	}
}

const std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() const
{
	return _gameObjects;
}
const std::vector<std::shared_ptr<GameObject>> Scene::getRenderableGameObjects() const
{
	return _renderableGameObjects;
}

void Scene::runCollisionUpdate()
{
	if(_boxCollisionComponents.size() > 0)
	{
		for(int i = 0; i < _boxCollisionComponents.size() - 1; i++)
		{
			for(int j = i + 1; j < _boxCollisionComponents.size(); j++)
			{
				//Try collision between i and j
				if(_boxCollisionComponents[i]->intersects(
					_boxCollisionComponents[j]))
				{
					//Create a new collision
					Collision c;
					//Forward to both game objects
					c.other = _boxCollisionComponents[i]->getGameObject();
					_boxCollisionComponents[j]->getGameObject()->onCollisionEnter(c);
					c.other = _boxCollisionComponents[j]->getGameObject();
					_boxCollisionComponents[i]->getGameObject()->onCollisionEnter(c);

				}
			}
		}		
	}
}

std::shared_ptr<GameObject> Scene::instantiate(GameObject * object, const Transform & newPosition)
{
	std::shared_ptr<GameObject>  newObject = std::make_shared<GameObject>();
	//First, create new components for object
	for(Component * c : object->getComponents())
	{
		std::shared_ptr<Component> comp = c->clone();
		_instantiateComponents.push_back(comp);
		newObject->addComponent(comp.get());
	}
	newObject->getTransform() = newPosition;
	_instantiateQueue.push_back(newObject);
	//Once the object is constructed, call 'start' on the new gameOjbect
	for(Component * c : newObject->getComponents())
	{
		c->start();
	}
	return newObject;

}

void Scene::instantiateNewObjects()
{
	if(_instantiateComponents.size() > 0 || _instantiateQueue.size() > 0)
	{
		LOG(INFO, "Instantiating new objects...");
		_components.insert(_components.end(),_instantiateComponents.begin(),_instantiateComponents.end());
		for(std::shared_ptr<GameObject> obj : _instantiateQueue)
		{
			addGameObject(obj);
		}
		_instantiateComponents.clear();
		_instantiateQueue.clear();	
	}

}

