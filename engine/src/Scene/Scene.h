#pragma once

#include "GameObject/GameObject.h"
#include "Util/Logger.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include <glm/glm.hpp>

#include <functional>

namespace MoonEngine
{
// <<<<<<< HEAD
// 	class Scene
// 	{
// 	public:
// 		Scene();
// 		/**
// 		 * Add a game object to the scene
// 		 * @depriciated use createGameObject() instead
// 		 * @param go Game engine
// 		 * @obsolte not be used
// 		 */
// 		void addGameObject(std::shared_ptr<GameObject> go);

// 		/**
// 		 * Find the first game object with a component
// 		 */
		
// 		template<class T>
// 		GameObject * findGameObjectWithComponent()
// 		{
// 			for(std::shared_ptr<GameObject> obj : _gameObjects)
// 			{
// 				if(obj->getComponent<T>() != nullptr)
// 				{
// 					return obj.get();
// 				}
// 			}
// 			LOG(WARN, "Could not find GameObject with component");
// 			return nullptr;
// 		}
// 		GameObject * findGameObjectWithTag(Tag t);

// 		float distanceFromFrutrum(glm::vec4 frustPlane, glm::vec3 point);
		
// 		const std::vector<std::shared_ptr<GameObject>> getGameObjects() const;
// 		const std::vector<std::shared_ptr<GameObject>> getRenderableGameObjects() const;
		
// 		std::shared_ptr<GameObject>  createGameObject()
// 		{
// 			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
// 			_allGameObjects.push_back(object);
// 			return object;
// 		}

		
// 		template< class T, class... Args >
// 		T * createComponent( Args&&... args )
// 		{
// 			std::shared_ptr<T> ptr = std::make_shared<T>(args...);
// 			_components.push_back(ptr);
// 			return ptr.get();
// 		}

// 		/**
// 		 * Create a new gameObject at a location
// 		 */
// 		std::shared_ptr<GameObject> instantiate(GameObject * object, const Transform & newTransform);
		
// 		/**
// 		 * Delete the gameObject from the scene
// 		 * @note: The gameObject will be valid until the end of frame.
// 		 * @param object the object to delete.
// 		 */
// 		void deleteGameObject(GameObject * object);

// 		/**
// 		 * Delete GameObjects from the scene post-update.
// 		 */
// 		void runDeleteGameObjects();


// 		/**
// 		 * Call the update() method of every game object
// 		 */
// 		void runUpdate(float dt);
// 		/**
// 		 * Perform collision detection, and call onCollisionEnter() methods
// 		 * after finishing collision detection.
// 		 */
// 		void runCollisionUpdate();

// 		void addCustomUpdate(std::function<void(float)> fn);

// 		void start();
// 		/**
// 		 * Cast a ray into the scene. Return
// 		 * true if a ray intersects that's
// 		 * less than maxLen
// 		 * @param  origin    the origin of the ray
// 		 * @param  direction the direction of the ray
// 		 * @param  maxLen    the maximum length of the ray
// 		 * @param  hit       information from a hit
// 		 * @return           true if hit.
// 		 */
// 		bool castRay(glm::vec3 origin, glm::vec3 direction, float maxLen = -1, Hit * hit = nullptr);

// 		float getGlobalTime();
// =======
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
         * Find the first game object with a component
         */

        template<class T>
        GameObject * findGameObjectWithComponent()
        {
            for (std::shared_ptr<GameObject> obj : _gameObjects)
            {
                if (obj->getComponent<T>() != nullptr)
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

        const std::vector<std::shared_ptr<GameObject>> getRenderableGameObjectsInFrustrum(glm::mat4 VP, Tag withTag = T_ALL) const;

        float distanceFromFrutrum(glm::vec4 frustPlane, glm::vec3 point);
        
        std::shared_ptr<GameObject> createGameObject()
        {
            std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
            _allGameObjects.push_back(object);
            return object;
        }
        std::shared_ptr<GameObject> createGameObject(const Transform & t)
        {
            std::shared_ptr<GameObject> object = std::make_shared<GameObject>(t);
            _allGameObjects.push_back(object);
            return object;
        }


        template<class T, class... Args>
        T * createComponent(Args && ... args)
        {
            std::shared_ptr<T> ptr = std::make_shared<T>(args...);
            _components.push_back(ptr);
            return ptr.get();
        }

        template<class T>
        T * cloneComponent(T * component)
        {
        	std::shared_ptr<T> ptr = std::static_pointer_cast<T>(component->clone());
        	_components.push_back(ptr);
        	return ptr.get();
        }
        template<class T>
        T * cloneComponent(std::shared_ptr<T> component)
        {
        	std::shared_ptr<T> ptr = std::static_pointer_cast<T>(component->clone());
        	_components.push_back(ptr);
        	return ptr.get();
        }

        /**
         * Create a new gameObject at a location
         */
        std::shared_ptr<GameObject> instantiate(GameObject * object, const Transform & newTransform);

        /**
         * Delete the gameObject from the scene
         * @note: The gameObject will be valid until the end of frame.
         * @param object the object to delete.
         */
        void deleteGameObject(GameObject * object);

        /**
         * Delete GameObjects from the scene post-update.
         */
        void runDeleteGameObjects();


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

        /**
         * Cast a ray into the scene. Return
         * true if a ray intersects that's
         * less than maxLen
         * @param  origin    the origin of the ray
         * @param  direction the direction of the ray
         * @param  maxLen    the maximum length of the ray
         * @param  hit       information from a hit
         * @return           true if hit.
         */
        bool castRay(glm::vec3 origin, glm::vec3 direction, float maxLen = -1, Hit * hit = nullptr);

        float getGlobalTime();


        glm::vec3 getGlobalLightDir();

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

        float _globalTime;
        glm::vec3 _globalLightDir;
    };
}
