#include "Material.h"
using namespace MoonEngine;

Material::Material(glm::vec3 tint):
	Component(),
	_tint(tint)
{
	
}

const glm::vec3 & Material::getTint() const
{
	return _tint;
}

void Material::setTint(glm::vec3 newTint)
{
	_tint = newTint;
}

