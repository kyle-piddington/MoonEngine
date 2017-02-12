#include "DirLight.h"


using namespace MoonEngine;

DirLight::DirLight(glm::vec3 color, glm::vec3 direction) :
    Light(color), _direction(direction)
{
}

void DirLight::update(glm::vec3 direction)
{
    _direction = direction;
}

const glm::vec3& DirLight::getDirection()
{
    return _direction;
}

std::shared_ptr<Component> DirLight::clone() const
{
    return std::make_shared<DirLight>(*this);
}
