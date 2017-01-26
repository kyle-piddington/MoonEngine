#include "BoxCollider.h"
#include "Component/MeshComponents/Mesh.h"
#include "GameObject/GameObject.h"
using namespace MoonEngine;
BoxCollider::BoxCollider():
_givenBox(false)
{

}

BoxCollider::BoxCollider(glm::vec3 minCoords, glm::vec3 maxCoords):
_givenBox(true)
{
	_originalBox = BoundingBox(minCoords.x, maxCoords.x, minCoords.y, maxCoords.y, minCoords.z, maxCoords.z);
}

void BoxCollider::start()
{
	if(!_givenBox)
	{
		//Retrieve mesh ptr
		Mesh * mesh = gameObject->getComponent<Mesh>();
		if(mesh == nullptr)
		{
			assert("!Box collider not given initial size, or mesh");
		}
		else
		{
			_originalBox = mesh->getMesh()->boundingBox;
		}
		_givenBox = true;
	}
}

void BoxCollider::update(float dt)
{
	_transformedBox = _originalBox.transform(gameObject->getTransform().getMatrix());
}

bool BoxCollider::intersects(const BoxCollider * other) 
{
	return _transformedBox.intersects(other->_transformedBox);
}

glm::vec3 BoxCollider::getCenter()
{
	return _transformedBox.centerPoint;
}

std::shared_ptr<Component> BoxCollider::clone() const
{
	return std::make_shared<BoxCollider>(*this);
}



