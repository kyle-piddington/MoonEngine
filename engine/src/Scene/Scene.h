#pragma once
#include "GameObject/GameObject.h"
#include "Util/Logger.h"
#include "Component/CollisionComponents/BoxCollider.h"
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
		std::shared_ptr<GameObject> findGameObjectWithComponent()
		{
			for(std::shared_ptr<GameObject> obj : _gameObjects)
			{
				if(obj->getComponent<T>() != nullptr)
				{
					return obj;
				}
			}
			LOG(WARN, "Could not find GameObject with component");
			return nullptr;
		}

		const std::vector<std::shared_ptr<GameObject>> getGameObjects() const;
		const std::vector<std::shared_ptr<GameObject>> getRenderableGameObjects() const;
		
		GameObject * createGameObject()
		{
			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			_allGameObjects.push_back(object);
			return object.get();
		}

		
		template< class T, class... Args >
		T * createComponent( Args&&... args )
		{
			std::shared_ptr<T> ptr = std::make_shared<T>(args...);
			_components.push_back(ptr);
			return ptr.get();
		}

		/**
		 * Call the update() method of every game object
		 */
		void runUpdate(float dt);
		/**
		 * Perform collision detection, and call onCollisionEnter() methods
		 * after finishing collision detection.
		 */
		void runCollisionUpdate();


		void start();

	private:
		//Poorly organized list for prototyping purposes.
		//Also contains 'prototype' game objects that won't be rendered.
		std::vector<std::shared_ptr<GameObject>> _allGameObjects;
		

		std::vector<std::shared_ptr<GameObject>> _gameObjects;
		std::vector<std::shared_ptr<GameObject>> _renderableGameObjects;

		//List of collision components
		////For performing box-box collisions
		std::vector<std::shared_ptr<BoxCollider>> _boxCollisionComponents;

		std::vector<std::shared_ptr<Component>> _components;


	};	
}
