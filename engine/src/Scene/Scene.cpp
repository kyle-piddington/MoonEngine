#include "Scene.h"
#include "Component/Components.h"
#include "Util/Logger.h"

#include <iostream>
using namespace MoonEngine;

#define TIME_MODIFIER 0.005

Scene::Scene()
{
    _globalTime = 0.0;
    _cameraFlag = 0;
    _dirLightFlag = 0;
    _allGameObjects.clear();
    _gameObjects.clear();
    _renderableGameObjects.clear();
    _forwardGameObjects.clear();
    _guiGameObjects.clear();
    _pointLightObjects.clear();
    _boxCollisionComponents.clear();
    _components.clear();
    _renderTree = nullptr;
    _playerObject = nullptr;
    _gameState = nullptr;
}

/**
 * Add the gameobject to the active scene.
 * @param obj [to add]
 */
void Scene::addGameObject(std::shared_ptr<GameObject> obj)
{
    _gameObjects.push_back(obj);
    if (obj->getTag() == T_GUI) {
        _guiGameObjects.push_back(obj);
    }
    if (obj->getComponent<GameState>() != nullptr) {
        _gameState = obj->getComponent<GameState>();
    }
    if (obj->getComponent<Camera>() != nullptr && _cameraFlag == 0) {
        _cameraFlag = 1;
        _mainCamera = obj;
        LOG(INFO, "Adding main camera object");
    }
    if (obj->getComponent<Material>() != nullptr &&
        obj->getComponent<Mesh>() != nullptr)
    {
        LOG(INFO, "Adding renderable game object");
        _renderableGameObjects.push_back(obj);

        if (obj->getComponent<Material>()->isForward())
        {
            LOG(INFO, "Adding forward game object");
            _forwardGameObjects.push_back(obj);
        }

        if(_renderTree != nullptr)
        {
            _renderTree->addObject(obj);
        }

    }

    if (obj->getComponent<PointLight>() != nullptr)
    {
        LOG(INFO, "Adding point light game object");
        _pointLightObjects.push_back(obj);
    }
    if (obj->getComponent<DirLight>() != nullptr && _dirLightFlag == 0)
    {
        LOG(INFO, "Adding dir light game object");
        _dirLightFlag = 1;
        _dirLightObject = obj;
    }
    else if(obj->getComponent<DirLight>() != nullptr && _dirLightFlag){
        LOG(WARN, "Attempting to add more than one directional light");
    }
    BoxCollider * col = obj->getComponent<BoxCollider>();
    if (col != nullptr)
    {
     LOG(INFO, "Adding collidable game object");
     _boxCollisionComponents.push_back(col);
 }
}

/**
 * Run updates for GameObjects, time, and collisions
 * @param dt [time change]
 */
void Scene::runUpdate(float dt)
{
    runCollisionUpdate();
    runMessageUpdate();

    if (_gameState != nullptr && _gameState->currentState() == PLAYING_STATE)
    {
        _globalTime += dt * TIME_MODIFIER;
    }
    instantiateNewObjects();

    for (std::shared_ptr<GameObject> go : _gameObjects)
    {
      go->update(dt);
    }
  
  if (updateFunctors.size() > 0)
  {
      for (auto & fun : updateFunctors)
      {
         fun(dt);
     }
 }
 _renderTree->update();

}

float Scene::getGlobalTime()
{
    return _globalTime;
}

void Scene::setGlobalTime(float time)
{
    _globalTime = time;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() const
{
    return _gameObjects;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getRenderableGameObjects() const
{
    return _renderableGameObjects;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getGuiGameObjects() const
{
    return _guiGameObjects;
}

std::vector<glm::vec4> getFrustrumPlanes(glm::mat4 comp)
{
	std::vector<glm::vec4> planes(6);
	glm::vec3 n;
	glm::vec4 Left, Right, Bottom, Top, Near, Far;
	n.x = comp[0][3] + comp[0][0];
	n.y = comp[1][3] + comp[1][0];
	n.z = comp[2][3] + comp[2][0];
	Left.w = comp[3][3] + comp[3][0];
	Left = planes[0] = glm::vec4(n, Left.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][0];
	n.y = comp[1][3] - comp[1][0];
	n.z = comp[2][3] - comp[2][0];
	Right.w = comp[3][3] - comp[3][0];
	Right = planes[1] = glm::vec4(n, Right.w)/ glm::length(n);

	n.x = comp[0][3] + comp[0][1];
	n.y = comp[1][3] + comp[1][1];
	n.z = comp[2][3] + comp[2][1];
	Bottom.w = comp[3][3] + comp[3][1];
	Bottom = planes[2] = glm::vec4(n, Bottom.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][1];
	n.y = comp[1][3] - comp[1][1];
	n.z = comp[2][3] - comp[2][1];
	Top.w = comp[3][3] - comp[3][1];
	Top = planes[3] = glm::vec4(n, Top.w)/ glm::length(n);

	n.x = comp[0][3] + comp[0][2];
	n.y = comp[1][3] + comp[1][2];
	n.z = comp[2][3] + comp[2][2];
	Near.w = comp[3][3] + comp[3][2];
	Near = planes[4] = glm::vec4(n, Near.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][2];
	n.y = comp[1][3] - comp[1][2];
	n.z = comp[2][3] - comp[2][2];
	Far.w = comp[3][3] - comp[3][2];
	Far = planes[5] = glm::vec4(n, Far.w) / glm::length(n);
	return planes;
}

float Scene::distanceFromFrutrum(glm::vec4 frustPlane, glm::vec3 point)
{
	return (frustPlane.x * point.x + frustPlane.y * point.y + frustPlane.z * point.z + frustPlane.w);
}

const std::vector<std::shared_ptr<GameObject>> Scene::getRenderableGameObjectsInFrustrum(glm::mat4 VP, Tag t) const
{
	std::vector<glm::vec4> planes = getFrustrumPlanes(VP);
	return _renderTree->getObjectsInFrustrum(planes);
}

const std::vector<std::shared_ptr<GameObject>> Scene::getForwardGameObjects() const
{
    return _forwardGameObjects;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getPointLightObjects() const
{
    return _pointLightObjects;
}

std::shared_ptr<GameObject> Scene::getDirLightObject() const
{
    return _dirLightObject;
}

void Scene::runCollisionUpdate()
{
    if (_playerObject != nullptr)
    {
        _renderTree->runCollisions(_playerObject);
    }
	//glm::vec3 colnormal;
   // if (_boxCollisionComponents.size() > 0)
    //{
      //  for (size_t i = 0; i < _boxCollisionComponents.size() - 1; i++)
       // {
        //    for (size_t j = i + 1; j < _boxCollisionComponents.size(); j++)
        //    {
                //Try collision between i and j
        //        if (_boxCollisionComponents[i]->intersects(
         //           _boxCollisionComponents[j], &colnormal))
        //        {
                    //Create a new collision
        //            Collision c;
         //           c.normal = colnormal;
         //           //Forward to both game objects
          //          c.other = _boxCollisionComponents[i]->getGameObject();
          //          _boxCollisionComponents[j]->getGameObject()->onCollisionEnter(c);
          //          c.normal = -colnormal;
           //         c.other = _boxCollisionComponents[j]->getGameObject();
         //           _boxCollisionComponents[i]->getGameObject()->onCollisionEnter(c);
//
        //        }
        //    }
       // }
  //  }
}

std::shared_ptr<GameObject> Scene::instantiate(GameObject * object, const Transform & newPosition)
{
    std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>();
    //First, create new components for object
    for (Component * c : object->getComponents())
    {
        std::shared_ptr<Component> comp = c->clone();
        _instantiateComponents.push_back(comp);
        newObject->addComponent(comp.get());
    }
    newObject->getTransform() = newPosition;
    _instantiateQueue.push_back(newObject);
    return newObject;

}

void Scene::deleteGameObject(shared_ptr<GameObject> object)
{
    //Add object to queue for deletion
    //On frame end, these objects should be deleted.
    object->setDeleted();
    std::vector<Component *> components = object->getComponents();
    int size = components.size();
    for (int i = 0; i < size; i++)
    {
        components.at(i)->setDeleted();
    }
}

void Scene::deleteGameObject(GameObject * object)
{
    //Add object to queue for deletion
    //On frame end, these objects should be deleted.
    object->setDeleted();
    std::vector<Component *> components = object->getComponents();
    int size = components.size();
    for (int i = 0; i < size; i++)
    {
        components.at(i)->setDeleted();
    }
}

void Scene::runDeleteGameObjects()
{
    //Delete all components on queued gameObjects
    //Also delete the gameObjects
    int size = _boxCollisionComponents.size();
    for (int i = 0; i < size; i++)
    {
        if (_boxCollisionComponents.at(i)->isDeleted())
        {
            //delete _boxCollisionComponents.at(i);
            _boxCollisionComponents.erase(_boxCollisionComponents.begin() + i);
            i--;
            size--;
        }
    }
    size = _components.size();
    for (int i = 0; i < size; i++)
    {
        if (_components.at(i) != nullptr && _components.at(i)->isDeleted())
        {
            _components.erase(_components.begin() + i);
            i--;
            size--;
        }
    }
    size = _allGameObjects.size();
    for (int i = 0; i < size; i++)
    {
        if (_allGameObjects.at(i) != nullptr && _allGameObjects.at(i)->isDeleted())
        {
            _allGameObjects.erase(_allGameObjects.begin() + i);
            i--;
            size--;
        }
    }
    size = _renderableGameObjects.size();
    for (int i = 0; i < size; i++)
    {
        if (_renderableGameObjects.at(i) != nullptr && _renderableGameObjects.at(i)->isDeleted())
        {
         _renderTree->removeObject(_renderableGameObjects.at(i));
         _renderableGameObjects.erase(_renderableGameObjects.begin() + i);
         i--;
         size--;
        }
    }

     size = _gameObjects.size();
     for (int i = 0; i < size; i++)
     {
        if (_gameObjects.at(i) != nullptr && _gameObjects.at(i)->isDeleted())
        {
            _gameObjects.erase(_gameObjects.begin() + i);
            i--;
            size--;
        }
    }
    size = _pointLightObjects.size();
    for (int i = 0; i < size; i++)
    {
        if (_pointLightObjects.at(i) != nullptr && _pointLightObjects.at(i)->isDeleted())
        {
            _pointLightObjects.erase(_pointLightObjects.begin() + i);
            i--;
            size--;
        }
    }
    size = _forwardGameObjects.size();
    for (int i = 0; i < size; i++)
    {
        if (_forwardGameObjects.at(i) != nullptr && _forwardGameObjects.at(i)->isDeleted())
        {
            _forwardGameObjects.erase(_forwardGameObjects.begin() + i);
            i--;
            size--;
        }
    }
    size = _guiGameObjects.size();
    for (int i = 0; i < size; i++)
    {
        if (_guiGameObjects.at(i) != nullptr && _guiGameObjects.at(i)->isDeleted())
        {
            _guiGameObjects.erase(_guiGameObjects.begin() + i);
            i--;
            size--;
        }
    }
}

std::shared_ptr<GameObject> MoonEngine::Scene::getMainCamera()
{
    return _mainCamera;
}

void Scene::instantiateNewObjects()
{
    if (_instantiateComponents.size() > 0 || _instantiateQueue.size() > 0)
    {
        LOG(INFO, "Instantiating new objects...");
        _components.insert(_components.end(), _instantiateComponents.begin(), _instantiateComponents.end());
        for (std::shared_ptr<GameObject> obj : _instantiateQueue)
        {
            addGameObject(obj);
            obj->start();
        }
        _instantiateComponents.clear();
        _instantiateQueue.clear();
    }

}

void Scene::addCustomUpdate(std::function<void(float)> fn)
{
    updateFunctors.push_back(fn);
}

//Naive implementation
GameObject * Scene::findGameObjectWithTag(Tag t)
{
    for (auto g : _gameObjects)
    {
        if (g->getTag() == t)
        {
            return g.get();
        }
    }
    return nullptr;
}

//Naive implementation
bool Scene::castRay(glm::vec3 origin, glm::vec3 direction, float maxDist, Hit * hit, Tag excludeTag)
{
    //Spatial data structure would go here.
    Hit tmpHit;
    float closestDist = FLT_MAX;
    if (glm::length(direction) > 0)
    {
        for (size_t i = 0; i < _boxCollisionComponents.size(); i++)
        {
         if (excludeTag & (_boxCollisionComponents[i]->getGameObject()->getTag()))
         {
            continue;
        }
        if (_boxCollisionComponents[i]->isTrigger)
        {
            continue;
        }
        Hit thisHit;

        if (_boxCollisionComponents[i]->intersectsRay(origin, direction, &thisHit))
        {

            if ((maxDist == -1 || thisHit.distance < maxDist) &&
               thisHit.distance  < closestDist)
            {
               tmpHit = thisHit;
               closestDist = thisHit.distance;
           }
       }
   }
   if (closestDist != FLT_MAX)
   {
    if(hit != nullptr)
    {
        *hit = tmpHit;
    }
    return true;
}

}
return false;

}


std::shared_ptr<GameObject> Scene::getPlayer()
{
    if(_playerObject != nullptr)
    {
        return _playerObject;
    }
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		if (T_Player == _gameObjects.at(i)->getTag())
		{
			return _gameObjects.at(i);
		}
	}
    return nullptr;
}

void Scene::start()
{
    // for(auto & g : _gameObjects)
    // {
    // //	g->start();
    // }
	_renderTree = std::make_shared<KDTree>(_renderableGameObjects);
	_playerObject = getPlayer();
}

void Scene::addGlobalMessage(const Message & message)
{
    _globalMessageQueue.push_back(message);
}

void Scene::addGlobalHandler(std::string message, const messageFn & fn)
{
    if (_globalMessageHandlers.find(message) == _globalMessageHandlers.end())
    {
        _globalMessageHandlers[message] = std::vector<messageFn>();
    }
    _globalMessageHandlers[message].push_back(fn);
}

void Scene::runMessageUpdate()
{
    // Send all the global messages
    std::vector<Message> bufferedMessage = _globalMessageQueue;
    _globalMessageQueue.clear();
    for (Message & msg : bufferedMessage)
    {
        if (_globalMessageHandlers.find(msg.message) != _globalMessageHandlers.end())
        {
            //If a handler for the message exists
            for (auto handler : _globalMessageHandlers[msg.message])
            {
                //call the handler
                handler(msg);
            }
        }
    }

}

void Scene::addPrefab(std::string name, GameObject * object)
{
	prefab = prefabMap({ { name, std::shared_ptr<GameObject>(object) } });
}

std::shared_ptr<GameObject> Scene::getPrefab(std::string name)
{
	if (prefab.find(name) != prefab.end())
	{
		return prefab.at(name);
	}
	return nullptr;
}
