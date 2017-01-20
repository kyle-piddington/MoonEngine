#pragma once
#include "GameObject/GameObject.h"
#include "Util/Logger.h"
namespace MoonEngine
{
	class Scene
	{
	public:
		/**
		 * Add a game object to the scene
		 * @param go Game engine
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
		
		/**
		 * Call the update() method of every game object
		 */
		void runUpdate(float dt);
		/**
		 * Perform collision detection, and call onCollisionEnter() methods
		 * after finishing collision detection.
		 */
		void runCollisionUpdate();

	private:
		//Poorly organized list for prototyping purposes.
		std::vector<std::shared_ptr<GameObject>> _gameObjects;
		std::vector<std::shared_ptr<GameObject>> _renderableGameObjects;


	};	
}
