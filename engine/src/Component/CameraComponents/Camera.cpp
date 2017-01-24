#include "Camera.h"
#include "GameObject/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace MoonEngine;
Camera::Camera(float fov, float aspect, float near, float far):
	_aspect(aspect),
	_near(near),
	_far(far),
	_fov(fov),
	_vNeedsUpdate(true)
	{
	}


void Camera::start()
{
	_P = glm::perspective(_fov, _aspect,  _near,  _far);
	_V = glm::inverse(gameObject->getTransform().getMatrix());
	_vNeedsUpdate = false;

}

void Camera::update(float dt)
{
	_vNeedsUpdate = true;
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
std::shared_ptr<Component> Camera::clone() const
{
	return std::make_shared<Camera>(*this);
}