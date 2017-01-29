#include "Material.h"
#include "Libraries/Library.h"
using namespace MoonEngine;

Material::Material(glm::vec3 tint, std::string program):
	Component(),
	_tint(tint)
{
	programPtr = Library::ProgramLib->getProgramForName(program);
	if(programPtr == nullptr)
	{
		programPtr = Library::ProgramLib->getProgramForName("default.program");
	}
}

const glm::vec3 & Material::getTint() const
{
	return _tint;
}

void Material::setTint(glm::vec3 newTint)
{
	_tint = newTint;
}


GLProgram * Material::getProgram() const
{
	return programPtr;
}



std::shared_ptr<Component> Material::clone() const
{
	return std::make_shared<Material>(*this);
}


