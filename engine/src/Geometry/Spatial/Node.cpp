#include "Node.h"
#include <algorithm>
#include "Collision/BoundingBox.h"
#include "Component/MeshComponents/Mesh.h"
#include "Component/CollisionComponents/BoxCollider.h"
using namespace MoonEngine;

Node::Node(std::vector<std::shared_ptr<GameObject>> gameObjects, int maxObjects, int axis, BoundingBox ourBoundary) :
	maxObjects(maxObjects),
	axis(axis),
	isLeaf(false),
	ourBoundary(ourBoundary)
{
	sortObjectsAndMakeChildren(gameObjects);
};

std::vector<std::shared_ptr<GameObject>> Node::getGameObjects()
{
	return gameObjects;
}

std::unordered_set<std::shared_ptr<GameObject>> Node::getObjectsInFrustrum(std::vector<glm::vec4> frust)
{
	std::unordered_set< std::shared_ptr<GameObject> > objectsInFrust;
	if (ourBoundary.inFrustrum(&frust.at(0)))
	{
		if (!isLeaf)
		{
			std::unordered_set< std::shared_ptr<GameObject> > left = leftChild->getObjectsInFrustrum(frust);
			std::unordered_set< std::shared_ptr<GameObject> > right = rightChild->getObjectsInFrustrum(frust);

			objectsInFrust.insert(left.begin(), left.end());
			objectsInFrust.insert(right.begin(), right.end());
		}
		else
		{
			float distance;
			glm::vec3 currBox[2];
			bool inside;
			for (int i = 0; i < gameObjects.size(); i++)
			{
				const BoundingBox & box =
					gameObjects.at(i)->getBounds();

				currBox[0] = (box.min());
				currBox[1] = (box.max());
				inside = true;
				for (int j = 0; j < 6; j++)
				{
					int ix = static_cast<int>(frust.at(j).x > 0.0f);
					int iy = static_cast<int>(frust.at(j).y > 0.0f);
					int iz = static_cast<int>(frust.at(j).z > 0.0f);

					distance = (frust.at(j).x * currBox[ix].x +
						frust.at(j).y * currBox[iy].y +
						frust.at(j).z * currBox[iz].z);
					if (distance < -frust.at(j).w)
					{
						inside = false;
						break;
					}
				}
				if (inside)
					objectsInFrust.insert(gameObjects.at(i));
			}
		}
	}
	
	return objectsInFrust;
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

void Node::median(const std::vector<std::shared_ptr<GameObject>> & gameObjects)
{
	int which = axis % 3;
	int size = gameObjects.size();
	if (which == 0)
	{
		std::vector<float> xList;
		for (int i = 0; i < size; i++)
		{
			xList.push_back(gameObjects.at(i)->getBounds().centerPoint.x);
		}
		std::sort(xList.begin(), xList.end());
		plane = glm::vec4(1.0f, 0.0f, 0.0f, -xList.at(size / 2));
		
	}
	else if (which == 1)
	{
		std::vector<float> yList;
		for (int i = 0; i < size; i++)
		{
			yList.push_back(gameObjects.at(i)->getBounds().centerPoint.y);
		}
		std::sort(yList.begin(), yList.end());
		plane = glm::vec4(0.0f, 1.0f, 0.0f, -yList.at(size / 2));
	}
	else
	{
		std::vector<float> zList;
		for (int i = 0; i < size; i++)
		{
			zList.push_back(gameObjects.at(i)->getBounds().centerPoint.z);
		}
		std::sort(zList.begin(), zList.end());
		plane = glm::vec4(0.0f, 0.0f, 1.0f, -zList.at(size / 2));
	}
}

void Node::sortObjectsAndMakeChildren(std::vector<std::shared_ptr<GameObject> > gameObjects)
{
	std::vector<std::shared_ptr<GameObject>> containedObjects = getFullyContainedObjects(gameObjects);
	if (maxObjects < containedObjects.size())
	{
		std::vector<std::shared_ptr<GameObject> > left, right;
		float distanceMax, distanceMin;
		glm::vec3 currBox[2];
		int size = gameObjects.size();
		median(containedObjects);
		glm::vec3 boxMin, newMin = ourBoundary.min();
		glm::vec3 boxMax, newMax = ourBoundary.max();
		int which = axis % 3;
		if (which == 0)
		{
			newMin.x += plane.w;
			newMax.x -= plane.w;
		}
		else if (which == 1)
		{
			newMin.y += plane.w;
			newMax.y -= plane.w;
		}
		else
		{
			newMin.z += plane.w;
			newMax.z -= plane.w;
		}
		BoundingBox leftBox(newMin.x, boxMax.x, newMin.y, boxMax.y, newMin.z, boxMax.z);
		BoundingBox rightBox(boxMin.x, newMax.x, boxMin.y, newMax.y, boxMin.z, newMax.z);
		int ix = static_cast<int>(plane.x > 0.0f);
		int iy = static_cast<int>(plane.y > 0.0f);
		int iz = static_cast<int>(plane.z > 0.0f);
		int mx = static_cast<int>(-plane.x > 0.0f);
		int my = static_cast<int>(-plane.y > 0.0f);
		int mz = static_cast<int>(-plane.z > 0.0f);
		for (int i = 0; i < size; i++)
		{
			const BoundingBox & box =
				gameObjects.at(i)->getBounds();

			currBox[0] = (box.min());
			currBox[1] = (box.max());

			distanceMax = (plane.x * currBox[ix].x +
				plane.y * currBox[iy].y +
				plane.z * currBox[iz].z);
			distanceMin = (plane.x * currBox[mx].x +
				plane.y * currBox[my].y +
				plane.z * currBox[mz].z);
			if (distanceMax >= -plane.w)
				left.push_back(gameObjects.at(i));
			if (distanceMin < -plane.w)
				right.push_back(gameObjects.at(i));
		}
		axis++;
		leftChild = std::make_shared<Node>(left, maxObjects, axis, leftBox);
		rightChild = std::make_shared<Node>(right, maxObjects, axis, rightBox);
	}
	else
	{
		isLeaf = true;
		this->gameObjects = gameObjects;
	}
}

std::vector<std::shared_ptr<GameObject>> Node::getFullyContainedObjects(const std::vector<std::shared_ptr<GameObject>> & allObjects)
{
	std::vector<std::shared_ptr<GameObject>> subObjects;
	for(auto go : allObjects)
	{
		if(ourBoundary.contains(go->getBounds()))
		{
			subObjects.push_back(go);
		}
	}
	return subObjects;
}
