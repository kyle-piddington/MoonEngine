#pragma once
#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <typeindex>
#include "Component.h"
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

		std::weak_ptr<T> getComponent()
		{
			if (componentMap.find(typeInfo(T)))
			{
				return static_cast<T>(componentMap(typeInfo(T)).front());
			}
			else
			{
				return nullptr;
			}
		}
		template <class T>
		std::vector<T> getComponent()
		{
			if (componentMap.find(typeInfo(T)))
			{
				return componentMap(typeInfo(T));
			}
		}

		void addComponent(Component * component)
		{
			componentMap[typeid(component)] = component;
		}
	private:
		/*
			Map of avaliable components
			If a component is de-allocated (Shouldn't happen often),
		*/
		std::unordered_map
			< std::type_index, Component *> componentMap;

	};

};