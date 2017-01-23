#pragma once
#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <vector>
#include "Tag.h"
#include "Component/Component.h"
#include "Geom/Transform.h"
/*
	Core GameObject component. Contains 
	a list of pointers to components, a tag,
	and methods to deal with message passing
*/
namespace MoonEngine
{

	class GameObject
	{
	public:
		GameObject();

		GameObject(const Transform & t);

		~GameObject();

		/**
		 * Initialize all components
		 */
		void start();

		void setParent(GameObject * otherObject);
		
		template <class T>
		T * getComponent()
		{
			//Look through components and cast to type.
			//(Could be slow, profile later)
			T* comp;
			for(Component * c : components)
			{
				if((comp = dynamic_cast<T *>(c)))
				{
					return comp;
				}
			}
			return nullptr;
		}

		void addComponent(Component * component);
		/**
		 * Get the transform of this game object
		 * @return a tranform object
		 */
		Transform & getTransform();
		/**
		 * Gather all components in the game object
		 */
		std::vector<Component *> getComponents();

		/**
		 * Run Update on all game objects
		 * @param dt delta time
		 */
		void update(float dt);
		/**
		 * Fire OnCollisionEnter on all components
		 * @param col the collision
		 */
		void onCollisionEnter(Collision col);
		/**
		 * Fire onCollisionExit on all components
		 * @param col the collision
		 */
		void onCollisionExit(Collision col);

	private:
		/*
			Map of avaliable components
			If a component is de-allocated (Shouldn't happen often),
		*/
		Transform transform;
		std::vector<Component *> components;
		std::vector<Tag> tags;
		GameObject * parent;

	};

};