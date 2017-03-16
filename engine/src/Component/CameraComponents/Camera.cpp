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
    _P = glm::perspective(_fov, _aspect, _near, _far);
    _V = glm::inverse(gameObject->getTransform().getMatrix());
    _vNeedsUpdate = false;

}

void Camera::update(float dt)
{
    _vNeedsUpdate = true;
}

const glm::mat4 & Camera::getProjection()
{
    return _P;
}

const glm::mat4 & Camera::getView()
{
    if (_vNeedsUpdate)
    {
        _V = glm::inverse(gameObject->getTransform().getMatrix());
    }
    return _V;
}

void Camera::setFOV(float fov)
{
    this->_fov = fov;
    _P = glm::perspective(fov, _aspect, _near, _far);
}

float Camera::getFOV() const{
    return this->_fov;
}

float Camera::getFar() const{
	return _far;
}
float Camera::getNear() const{
	return _near;
}

float MoonEngine::Camera::getAspect() const
{
    return this->_aspect;
}

void Camera::getPlanes(glm::vec4 planes[6])
{
	glm::vec3 n;
	glm::vec4 Left, Right, Bottom, Top, Near, Far;
	glm::mat4 comp = getProjection() * getView();
	n.x = comp[0][3] + comp[0][0];
	n.y = comp[1][3] + comp[1][0];
	n.z = comp[2][3] + comp[2][0];
	Left.w = comp[3][3] + comp[3][0];
	Left = planes[0] = glm::vec4(n, Left.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][0];
	n.y = comp[1][3] - comp[1][0];
	n.z = comp[2][3] - comp[2][0];
	Right.w = comp[3][3] - comp[3][0];
	Right = planes[1] = glm::vec4(n, Right.w)/ glm::length(n);

	n.x = comp[0][3] + comp[0][1];
	n.y = comp[1][3] + comp[1][1];
	n.z = comp[2][3] + comp[2][1];
	Bottom.w = comp[3][3] + comp[3][1];
	Bottom = planes[2] = glm::vec4(n, Bottom.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][1];
	n.y = comp[1][3] - comp[1][1];
	n.z = comp[2][3] - comp[2][1];
	Top.w = comp[3][3] - comp[3][1];
	Top = planes[3] = glm::vec4(n, Top.w)/ glm::length(n);

	n.x = comp[0][3] + comp[0][2];
	n.y = comp[1][3] + comp[1][2];
	n.z = comp[2][3] + comp[2][2];
	Near.w = comp[3][3] + comp[3][2];
	Near = planes[4] = glm::vec4(n, Near.w)/ glm::length(n);

	n.x = comp[0][3] - comp[0][2];
	n.y = comp[1][3] - comp[1][2];
	n.z = comp[2][3] - comp[2][2];
	Far.w = comp[3][3] - comp[3][2];
	Far = planes[5] = glm::vec4(n, Far.w) / glm::length(n);
}

std::shared_ptr<Component> Camera::clone() const
{
    return std::make_shared<Camera>(*this);
}