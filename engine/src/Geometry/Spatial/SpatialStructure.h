#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "GameObject\GameObject.h"

namespace MoonEngine
{
	class SpatialStructure
	{
	public:
		virtual void init(std::vector<std::shared_ptr<GameObject>> gameObjects);
		virtual void handleCollsion(std::shared_ptr<GameObject> gameObjects);
	private:
		std::shared_ptr<Node> head;

	};
}