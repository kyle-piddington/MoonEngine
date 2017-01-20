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
		Component(std::shared_ptr<GameObject> object);
		virtual ~Component();
		/*
			Take a reference to this gameobject, and pass
			a reference to this component to the GameObject.
		*/
		void provideGameObject(std::shared_ptr<GameObject> object);
	

		//Base component functions
		virtual void update(float dt);

		virtual void onCollisionEnter(Collision col);

		virtual void onCollisionExit(Collision col);

	protected:
		std::shared_ptr<GameObject> gameObject;

	};
};

