#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "GameObject\GameObject.h"

namespace MoonEngine
{
	class KDTree
	{
	public:
		KDTree(std::vector<std::shared_ptr<GameObject>> GameObject);
		std::unordered_set<std::shared_ptr<GameObject>> getObjectsInFrustrum(glm::vec4 frust[6]);

	private:
		std::shared_ptr<Node> head;
		std::vector<std::shared_ptr<Node>> regions;
	};
}