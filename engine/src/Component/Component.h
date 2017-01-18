#pragma once
/** 
	Abstract component class, implements behavoirs
	a message queue for handling internal messages
*/

#include <memory>

namespace MoonEngine
{
	class GameObject;
	class Component
	{
	public:
		/*Create a new component, and bind it to a gameObject*/
		Component(std::shared_ptr<GameObject> object);
		~Component();
		/*
			Take a reference to this gameobject, and pass
			a reference to this component to the GameObject.
		*/
		void provideGameObject(std::shared_ptr<GameObject> object);
	protected:
		std::shared_ptr<GameObject> gameObject;

	};
};

