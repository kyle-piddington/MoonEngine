#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "GameObject\GameObject.h"

namespace MoonEngine
{
	class Node
	{
	public:
		Node(std::vector<std::shared_ptr<GameObject>> gameObjects, int maxObjects, int axis);
		std::vector<std::shared_ptr<GameObject>> getGameObjects();
		std::shared_ptr<Node> getLeftChild();
		std::shared_ptr<Node> getRightChild();
		void setLeftChild(std::shared_ptr<Node> n);
		void setRightChild(std::shared_ptr<Node> n);
		void sortObjectsAndMakeChildren();
	private:
		void median();
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		glm::vec4 plane;
		std::shared_ptr<Node> leftChild;
		std::shared_ptr<Node> rightChild;
		int maxObjects;
		int axis;
	};
}
