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


float Camera::getFar() const{
	return _far;
}
float Camera::getNear() const{
	return _near;
}

void Camera::getPlanes(glm::vec4 planes[])
{
	glm::vec3 n;
	glm::vec4 Left, Right, Bottom, Top, Near, Far;
	glm::mat4 comp = getProjection() * getView();
	n.x = comp[0][3] + comp[0][0]; // see handout to fill in with values from comp
	n.y = comp[1][3] + comp[1][0]; // see handout to fill in with values from comp
	n.z = comp[2][3] + comp[2][0]; // see handout to fill in with values from comp
	Left.w = comp[3][3] + comp[3][0]; // see handout to fill in with values from comp
	Left = planes[0] = glm::vec4(n, Left.w)/ glm::length(n);
	//cout << "Left' " << Left.x << " " << Left.y << " " << Left.z << " " << Left.w << endl;

	n.x = comp[0][3] - comp[0][0]; // see handout to fill in with values from comp
	n.y = comp[1][3] - comp[1][0]; // see handout to fill in with values from comp
	n.z = comp[2][3] - comp[2][0]; // see handout to fill in with values from comp
	Right.w = comp[3][3] - comp[3][0]; // see handout to fill in with values from comp
	Right = planes[1] = glm::vec4(n, Right.w)/ glm::length(n);
	//cout << "Right " << Right.x << " " << Right.y << " " << Right.z << " " << Right.w << endl;

	n.x = comp[0][3] + comp[0][1]; // see handout to fill in with values from comp
	n.y = comp[1][3] + comp[1][1]; // see handout to fill in with values from comp
	n.z = comp[2][3] + comp[2][1]; // see handout to fill in with values from comp
	Bottom.w = comp[3][3] + comp[3][1]; // see handout to fill in with values from comp
	Bottom = planes[2] = glm::vec4(n, Bottom.w)/ glm::length(n);
	//cout << "Bottom " << Bottom.x << " " << Bottom.y << " " << Bottom.z << " " << Bottom.w << endl;

	n.x = comp[0][3] - comp[0][1];// see handout to fill in with values from comp
	n.y = comp[1][3] - comp[1][1]; // see handout to fill in with values from comp
	n.z = comp[2][3] - comp[2][1]; // see handout to fill in with values from comp
	Top.w = comp[3][3] - comp[3][1]; // see handout to fill in with values from comp
	Top = planes[3] = glm::vec4(n, Top.w)/ glm::length(n);
	//cout << "Top " << Top.x << " " << Top.y << " " << Top.z << " " << Top.w << endl;

	n.x = comp[0][3] + comp[0][2]; // see handout to fill in with values from comp
	n.y = comp[1][3] + comp[1][2]; // see handout to fill in with values from comp
	n.z = comp[2][3] + comp[2][2]; // see handout to fill in with values from comp
	Near.w = comp[3][3] + comp[3][2]; // see handout to fill in with values from comp
	Near = planes[4] = glm::vec4(n, Near.w)/ glm::length(n);
	//cout << "Near " << Near.x << " " << Near.y << " " << Near.z << " " << Near.w << endl;

	n.x = comp[0][3] - comp[0][2]; // see handout to fill in with values from comp
	n.y = comp[1][3] - comp[1][2]; // see handout to fill in with values from comp
	n.z = comp[2][3] - comp[2][2]; // see handout to fill in with values from comp
	Far.w = comp[3][3] - comp[3][2]; // see handout to fill in with values from comp
	Far = planes[5] = glm::vec4(n, Far.w) / glm::length(n);
}

std::shared_ptr<Component> Camera::clone() const
{
    return std::make_shared<Camera>(*this);
}