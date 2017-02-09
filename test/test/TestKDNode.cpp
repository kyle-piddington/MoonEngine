#include "catch.hpp"
#include "MoonEngine.h"
#include <sstream>
#include <iostream>
using namespace MoonEngine;

TEST_CASE("KDTree stores an object", "[Node]")
{
	BoxCollider boxCol(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Transform ourTrans;
	ourTrans.setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
	std::vector<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(ourTrans);
	obj->addComponent(&boxCol);
	objects.push_back(obj);
	Node n(objects, 1, 0, BoundingBox(-10, 10, -10, 10, -10, 10));
	REQUIRE(n.getGameObjects().size() == 1);
}
TEST_CASE("KDTree builds and separates itself", "[Node]")
{
	BoxCollider boxCol(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Transform ourTrans, theirTrans;
	ourTrans.setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
	theirTrans.setPosition(glm::vec3(-2.0f, 1.0f, 1.0f));
	std::vector<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(ourTrans);
	obj->addComponent(&boxCol);
	objects.push_back(obj);
	std::shared_ptr<GameObject> objB = std::make_shared<GameObject>(theirTrans);
	objB->addComponent(&boxCol);
	objects.push_back(objB);
	Node n(objects, 1, 0, BoundingBox(-10, 10, -10, 10, -10, 10));
	REQUIRE(n.getLeftChild() != nullptr);
	REQUIRE(n.getRightChild() != nullptr);
	REQUIRE(n.getLeftChild()->getGameObjects().size() == 1);
}