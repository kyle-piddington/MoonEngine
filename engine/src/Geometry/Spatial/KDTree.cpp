#include "KDTree.h"
#include "Util/Logger.h"
#include "GameObject/Tag.h"
#include <iostream>
using namespace MoonEngine;

KDTree::KDTree(std::vector<std::shared_ptr<GameObject>> gameObjects)
{
	head = std::make_shared<Node>(gameObjects, 5, 0, BoundingBox(-500, 500, -250, 250, -500, 500));
	LOG(GAME, "Maximum depth of KD tree: "  + std::to_string(head->getMaximumDepth()));
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (((T_Player | T_Dynamic) & gameObjects.at(i)->getTag()) > 0)
			dynamicObjects.push_back(gameObjects.at(i));
	}
	//regions = head->gatherChildren();
}

std::vector<std::shared_ptr<GameObject>> KDTree::getObjectsInFrustrum(std::vector<glm::vec4> frust)
{
	std::unordered_set<std::shared_ptr<GameObject>> inFrust = head->getObjectsInFrustrum(frust);
	return std::vector<std::shared_ptr<GameObject>>(inFrust.begin(), inFrust.end());
}

void KDTree::addObject(std::shared_ptr<GameObject> gameObject)
{
	if((T_Player | T_Dynamic) & gameObject->getTag())
	{
		addDynamicObject(gameObject);
	}
	else
	{
		head->add(gameObject);		
	}

}

void KDTree::removeObject(std::shared_ptr<GameObject> gameObject)
{
	head->remove(gameObject);
}

void KDTree::setDynamicObjects(std::vector<std::shared_ptr<GameObject>> dynamicObjects)
{
	this->dynamicObjects = dynamicObjects;
}

void KDTree::addDynamicObject(std::shared_ptr<GameObject> dynamicObject)
{
	dynamicObjects.push_back(dynamicObject);
	head->add(dynamicObject);		
}

void KDTree::update()
{
	bool has;
	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		std::vector<Node *> region = dynamicObjects.at(i)->getNodes();
		bool removed = false;
		for (int j = 0; j < region.size(); j++)
		{
			if (!region.at(j)->getBoundary().contains(dynamicObjects.at(i)->getBounds()))
			{
				//I know it repeats if we aren't within, but colliding with, a Node, but it shouldn't be too great of an 
				//affect on performance.
				region.at(j)->remove(dynamicObjects.at(i));
				region.erase(region.begin() + j);
				removed = true;
				//std::cout << "Moved!" << j << std::endl;
				j--;
				
			}
			
		}
		if (removed)
		{
			head->add(dynamicObjects.at(i));
			//std::cout << "Moved!" << i << std::endl;
		}
	}
}