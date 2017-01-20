#include "Scene.h"
#include "Component/Components.h"
using namespace MoonEngine;


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
	//Collision Code goes here :)
}

