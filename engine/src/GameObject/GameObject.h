#pragma once
#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <typeindex>
#include <vector>
#include "Component/Component.h"
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

		~GameObject();
		
		template <class T>
		T * getComponent()
		{
			//Look through components and cast to type.
			//(Could be slow, profile later)
			T* comp;
			for(Component * c : components)
			{
				if((comp = dynamic_cast<T>(c)))
				{
					return comp;
				}
			}
			return nullptr;
		}

		void addComponent(Component * component)
		{
			components.push_back(component);
		}
	private:
		/*
			Map of avaliable components
			If a component is de-allocated (Shouldn't happen often),
		*/
		std::vector<Component *> components;

	};

};