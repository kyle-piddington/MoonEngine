#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "GameObject/GameObject.h"
#include "Collision/BoundingBox.h"
namespace MoonEngine
{
	class Node
	{
	public:
		Node(std::vector<std::shared_ptr<GameObject>> gameObjects, int maxObjects, int axis, BoundingBox ourBoundary, int depth = 0);
		std::vector<std::shared_ptr<GameObject>> getGameObjects();
		std::unordered_set<std::shared_ptr<GameObject>> getObjectsInFrustrum(std::vector<glm::vec4> frust);
		std::shared_ptr<Node> getLeftChild();
		std::shared_ptr<Node> getRightChild();
		void setLeftChild(std::shared_ptr<Node> n);
		void setRightChild(std::shared_ptr<Node> n);
		void sortObjectsAndMakeChildren(std::vector<std::shared_ptr<GameObject>> gameObjects);
		int getMaximumDepth() const;
	private:
		std::vector<std::shared_ptr<GameObject>> getFullyContainedObjects(const std::vector<std::shared_ptr<GameObject>> & allObjects);
		void median(const std::vector<std::shared_ptr<GameObject>>  &gameObjects);
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		glm::vec4 plane;
		std::shared_ptr<Node> leftChild;
		std::shared_ptr<Node> rightChild;
		int maxObjects;
		int axis;
		bool isLeaf;
		int depth;
		BoundingBox ourBoundary;
	};
}
