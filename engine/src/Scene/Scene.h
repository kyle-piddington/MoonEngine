#pragma once
#include "GameObject/GameObject.h"
#include "Util/Logger.h"
#include "Component/CollisionComponents/BoxCollider.h"

#include <functional>
namespace MoonEngine
{
	class Scene
	{
	public:
		Scene();
		/**
		 * Add a game object to the scene
		 * @depriciated use createGameObject() instead
		 * @param go Game engine
		 * @obsolte not be used
		 */
		void addGameObject(std::shared_ptr<GameObject> go);

		/**
		 * Find the first game object with a componentt
		 */
		
		template<class T>
		GameObject * findGameObjectWithComponent()
		{
			for(std::shared_ptr<GameObject> obj : _gameObjects)
			{
				if(obj->getComponent<T>() != nullptr)
				{
					return obj.get();
				}
			}
			LOG(WARN, "Could not find GameObject with component");
			return nullptr;
		}
		GameObject * findGameObjectWithTag(Tag t);
		
		const std::vector<std::shared_ptr<GameObject>> getGameObjects() const;
		const std::vector<std::shared_ptr<GameObject>> getRenderableGameObjects() const;
		
		std::shared_ptr<GameObject>  createGameObject()
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			_allGameObjects.push_back(object);
			return object;
		}

		
		template< class T, class... Args >
		T * createComponent( Args&&... args )
		{
			std::shared_ptr<T> ptr = std::make_shared<T>(args...);
			_components.push_back(ptr);
			return ptr.get();
		}

		std::shared_ptr<GameObject> instantiate(GameObject * object, const Transform & newTransform);
		
		/**
		 * Call the update() method of every game object
		 */
		void runUpdate(float dt);
		/**
		 * Perform collision detection, and call onCollisionEnter() methods
		 * after finishing collision detection.
		 */
		void runCollisionUpdate();

		void addCustomUpdate(std::function<void(float)> fn);

		void start();

	private:

		void instantiateNewObjects();
		//Poorly organized list for prototyping purposes.
		//Also contains 'prototype' game objects that won't be rendered.
		std::vector<std::shared_ptr<GameObject>> _allGameObjects;
		

		std::vector<std::shared_ptr<GameObject>> _gameObjects;
		std::vector<std::shared_ptr<GameObject>> _renderableGameObjects;

		//List of collision components
		////For performing box-box collisions
		///warning - exceedingly terrible design.
		std::vector<BoxCollider *> _boxCollisionComponents;

		std::vector<std::shared_ptr<Component>> _components;

		//Insert queue
		std::vector<std::shared_ptr<GameObject>> _instantiateQueue;
		std::vector<std::shared_ptr<Component>> _instantiateComponents;
		

		//Custom update
		std::vector<std::function<void(float)>> updateFunctors;

	};	
}
