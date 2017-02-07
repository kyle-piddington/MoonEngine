#include "Node.h"
#include <algorithm>
#include "Collision\BoundingBox.h"
#include "Component\MeshComponents\Mesh.h"
#include "Component\CollisionComponents\BoxCollider.h"
using namespace MoonEngine;

Node::Node(std::vector<std::shared_ptr<GameObject>> gameObjects, int maxObjects, int axis) :
	gameObjects(gameObjects),
	maxObjects(maxObjects),
	axis(axis)
{
	sortObjectsAndMakeChildren();
};

std::vector<std::shared_ptr<GameObject>> Node::getGameObjects()
{
	return gameObjects;
}

std::shared_ptr<Node> Node::getLeftChild()
{
	return leftChild;
}

std::shared_ptr<Node> Node::getRightChild()
{
	return rightChild;
}

void Node::setLeftChild(std::shared_ptr<Node> n)
{
	leftChild = n;
}

void Node::setRightChild(std::shared_ptr<Node> n)
{
	rightChild = n;
}

void Node::median()
{
	int which = axis % 3;
	int size = gameObjects.size();
	if (!axis)
	{
		std::vector<float> xList;
		for (int i = 0; i < size; i++)
		{
			xList.push_back(gameObjects.at(i)->getComponent<BoxCollider>()->getBoundingBox().centerPoint.x);
		}
		std::sort(xList.begin(), xList.end());
		plane = glm::vec4(1.0f, 0.0f, 0.0f, -xList.at(size / 2));
		
	}
	else if (axis == 1)
	{
		std::vector<float> yList;
		for (int i = 0; i < size; i++)
		{
			yList.push_back(gameObjects.at(i)->getComponent<BoxCollider>()->getBoundingBox().centerPoint.y);
		}
		std::sort(yList.begin(), yList.end());
		plane = glm::vec4(1.0f, 0.0f, 0.0f, -yList.at(size / 2));
	}
	else
	{
		std::vector<float> zList;
		for (int i = 0; i < size; i++)
		{
			zList.push_back(gameObjects.at(i)->getComponent<BoxCollider>()->getBoundingBox().centerPoint.z);
		}
		std::sort(zList.begin(), zList.end());
		plane = glm::vec4(1.0f, 0.0f, 0.0f, -zList.at(size / 2));
	}
}

void Node::sortObjectsAndMakeChildren()
{
	if (maxObjects >= gameObjects.size())
	{
		std::vector<std::shared_ptr<GameObject>> left, right;
		float distance;
		glm::vec3 currBox[2];
		int size = gameObjects.size();
		int ix = static_cast<int>(plane.x > 0.0f);
		int iy = static_cast<int>(plane.y > 0.0f);
		int iz = static_cast<int>(plane.z > 0.0f);
		for (int i = 0; i < size; i++)
		{
			const BoundingBox & box =
				gameObjects.at(i)->getComponent<Mesh>()->getBoundingBox();

			currBox[0] = (box.min());
			currBox[1] = (box.max());

			distance = (plane.x * currBox[ix].x +
				plane.y * currBox[iy].y +
				plane.z * currBox[iz].z);
			if (distance < -plane.w)
				right.push_back(gameObjects.at(i));
			else
				left.push_back(gameObjects.at(i));
		}
		median();
		axis++;
		leftChild = std::make_shared<Node>(left, maxObjects, axis);
		rightChild = std::make_shared<Node>(right, maxObjects, axis);
	}
}