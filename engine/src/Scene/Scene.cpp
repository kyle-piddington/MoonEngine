#include "Scene.h"
#include "Component/Components.h"
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
		_renderableGameObjects.push_back(obj);
	}
}

void Scene::runUpdate(float dt)
{
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
			for(int j = i; j < _boxCollisionComponents.size(); j++)
			{
				//Try collision between i and j
				if(_boxCollisionComponents[i]->intersects(
					_boxCollisionComponents[j].get()))
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

