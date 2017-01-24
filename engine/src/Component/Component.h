#pragma once
/** 
	Abstract component class, implements behavoirs
	a message queue for handling internal messages
*/

#include <memory>
#include "Collision/Collision.h"
namespace MoonEngine
{
	class GameObject;
	class Component
	{
	public:
		
		/*Create a new component, and bind it to a gameObject*/
		Component();
		virtual ~Component();
		/*
			Take a reference to this gameobject, and pass
			a reference to this component to the GameObject.
		*/
		void provideGameObject(GameObject * object);
		
		/**
		 * Copy a component
		 * @return a shared pointer to a new component
		 */
		virtual std::shared_ptr<Component> clone() const = 0;

		virtual void start();
		//Base component functions
		virtual void update(float dt);

		virtual void onCollisionEnter(Collision col);

		virtual void onCollisionExit(Collision col);

		GameObject * getGameObject(){
			return gameObject;
		}
	protected:
		GameObject * gameObject;

	};
};

