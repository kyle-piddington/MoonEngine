#include "Camera.h"
#include "GameObject/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace MoonEngine;
Camera::Camera(std::shared_ptr<GameObject> obj, float fov, float aspect, float near, float far):
	Component(obj),
	_aspect(aspect),
	_near(near),
	_far(far),
	_fov(fov)
	{
		_P = glm::perspective(fov, aspect,  near,  far);
		_V = glm::inverse(gameObject->getTransform().getMatrix());
		_vNeedsUpdate = false;
	}
const glm::mat4  & Camera::getProjection()
{
	return _P;
}

const glm::mat4 & Camera::getView()
{
	if(_vNeedsUpdate)
	{
		_V = glm::inverse(gameObject->getTransform().getMatrix());
	}
	return _V; 
}

void Camera::setFOV(float fov)
{
	this->_fov= fov;
	_P = glm::perspective(fov, _aspect,_near, _far);
}