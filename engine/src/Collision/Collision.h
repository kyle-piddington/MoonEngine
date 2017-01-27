#pragma once
#include <glm/glm.hpp>

namespace MoonEngine
{
	//Forward declaration because this
	//dependency is awful.
	class GameObject;

	class Collision
	{
	public:
		GameObject * other;
		glm::vec3 normal;
	};	
}
