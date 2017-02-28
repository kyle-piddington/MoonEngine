#pragma once
#include <functional>
#include <string>
/**
 * Messages are sent through a gameObject and are processed at the start of each gameObjects'
 * update() order. Messages are formatted in a queue to stop over-messaging.
 */

namespace MoonEngine
{
	class GameObject;
	class Component;
	struct Message
	{
		std::string message;
		GameObject * sendingObject;
		Component * sendingComponent;
	};
	typedef std::function<void(const Message &)> messageFn;	
}
