#include "DirLight.h"


using namespace MoonEngine;

DirLight::DirLight(glm::vec3 color, glm::vec3 direction, std::string program) :
    Light(color), _direction(direction)
{
    _program = Library::ProgramLib->getProgramForName(program);
}

void DirLight::update(glm::vec3 direction)
{
    _direction = direction;
}

glm::vec3 DirLight::getDirection()
{
    return _direction;
}

GLProgram* DirLight::getProgram()
{
    return _program;
}
