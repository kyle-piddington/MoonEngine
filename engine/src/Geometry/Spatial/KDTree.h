#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "GameObject/GameObject.h"

namespace MoonEngine
{
	class KDTree
	{
	public:
		KDTree(std::vector<std::shared_ptr<GameObject>> gameObjects);
		std::vector<std::shared_ptr<GameObject>> getObjectsInFrustrum(std::vector<glm::vec4> frust);
		//void runCollisions(std::shared_ptr<GameObject> gameObject);
	private:
		std::shared_ptr<Node> head;
		std::vector<std::shared_ptr<Node>> regions;
	};
}