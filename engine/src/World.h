#pragma once
/**
World contains game constants for world coordinate systems
*/
#include <glm/glm.hpp>
namespace MoonEngine
{
	namespace World
	{
		glm::vec3 Up = glm::vec3(0, 1, 0);
		glm::vec3 Right = glm::vec3(1, 0, 0);
		glm::vec3 Forward = glm::vec3(0, 0, 1);

	};
}
