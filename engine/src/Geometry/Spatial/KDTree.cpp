#include "KDTree.h"

using namespace MoonEngine;

KDTree::KDTree(std::vector<std::shared_ptr<GameObject>> gameObjects)
{
	head = std::make_shared<Node>(gameObjects, 5, 0, BoundingBox(-500, 500, 0, 250, -500, 500));
}

std::vector<std::shared_ptr<GameObject>> KDTree::getObjectsInFrustrum(std::vector<glm::vec4> frust)
{
	std::unordered_set<std::shared_ptr<GameObject>> inFrust = head->getObjectsInFrustrum(frust);
	return std::vector<std::shared_ptr<GameObject>>(inFrust.begin(), inFrust.end());
}

