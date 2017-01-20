#include "Camera.h"
#include "GameObject/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace MoonEngine;
Camera::Camera(std::shared_ptr<GameObject> obj, float fov, float aspect, float near, float far):
	Component(obj),
	aspect(aspect),
	near(near),
	far(far),
	fov(fov)
	{
		P = glm::perspective(fov, aspect,  near,  far);
		V = glm::inverse(gameObject->getTransform().getMatrix());
		vNeedsUpdate = false;
	}
const glm::mat4  & Camera::getProjection()
{
	return P;
}

const glm::mat4 & Camera::getView()
{
	if(vNeedsUpdate)
	{
		V = glm::inverse(gameObject->getTransform().getMatrix());
	}
	return V; 
}

void Camera::setFOV(float fov)
{
	this->fov= fov;
	P = glm::perspective(fov, aspect,near,far);
}