#include "Material.h"
using namespace MoonEngine;

Material::Material(std::shared_ptr<GameObject> obj, glm::vec3 tint):
	Component(obj),
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

