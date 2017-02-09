#include "DirLight.h"

MoonEngine::DirLight::DirLight(glm::vec3 color, glm::vec3 direction) :
    Light(color), _direction(direction)
{
}

void MoonEngine::DirLight::update(glm::vec3 direction)
{
    _direction = direction;
}

glm::vec3 MoonEngine::DirLight::getDirection()
{
    return _direction;
}
