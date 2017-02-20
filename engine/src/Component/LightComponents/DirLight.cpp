#include "DirLight.h"


using namespace MoonEngine;

DirLight::DirLight(glm::vec3 direction, glm::vec3 color, float ambient, float intensity) :
    Light(color, ambient), _direction(direction), _intensity(intensity)
{
}

void DirLight::update(float dt)
{
    //_direction = direction;
}

const glm::vec3& DirLight::getDirection()
{
    return _direction;
}

std::shared_ptr<Component> DirLight::clone() const
{
    return std::make_shared<DirLight>(*this);
}
