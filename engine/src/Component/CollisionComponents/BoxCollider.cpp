#include "BoxCollider.h"
#include "Component/MeshComponents/Mesh.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;

BoxCollider::BoxCollider():
    _givenBox(false),
	isTrigger(false)
{
}

BoxCollider::BoxCollider(glm::vec3 minCoords, glm::vec3 maxCoords) :
	_givenBox(true),
	isTrigger(false)
{
    _originalBox = BoundingBox(minCoords.x, maxCoords.x, minCoords.y, maxCoords.y, minCoords.z, maxCoords.z);
}

void BoxCollider::start()
{
    if (!_givenBox)
    {
        //Retrieve mesh ptr
        Mesh * mesh = gameObject->getComponent<Mesh>();
        if (mesh == nullptr)
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

bool BoxCollider::intersects(const BoxCollider * other, glm::vec3 * colnormal)
{
    return _transformedBox.intersects(other->_transformedBox, colnormal);
}


bool BoxCollider::intersectsRay(glm::vec3 origin, glm::vec3 direction, Hit * hit)
{
    glm::vec3 nor;
    float hitDist = _transformedBox.intersectsRay(origin, direction, &nor);
    if (hitDist >= 0)
    {
        if (hit != nullptr)
        {
            hit->object = gameObject;
            hit->intersectionPoint = origin + direction * hitDist;
            hit->intersectionNormal = nor;
            hit->distance = hitDist;

        }
        return true;
    }
    return false;
}

glm::vec3 BoxCollider::getCenter()
{
    return _transformedBox.centerPoint;
}

glm::vec3 BoxCollider::getHalfWidths()
{
    return
            glm::vec3(_transformedBox.xHalfWidth, _transformedBox.yHalfWidth, _transformedBox.zHalfWidth);
}

const BoundingBox & BoxCollider::getBoundingBox()
{
	return _transformedBox;
}

std::shared_ptr<Component> BoxCollider::clone() const
{
    return std::make_shared<BoxCollider>(*this);
}