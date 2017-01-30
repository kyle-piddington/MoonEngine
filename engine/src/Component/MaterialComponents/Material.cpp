#include "Material.h"
#include "Libraries/Library.h"
using namespace MoonEngine;

Material::Material(glm::vec3 tint, std::string program, unordered_map<MaterialProperty, string> textures):
	Component(),
	_tint(tint)
{
	_programPtr = Library::ProgramLib->getProgramForName(program);
	if(_programPtr == nullptr)
	{
		_programPtr = Library::ProgramLib->getProgramForName("default.program");
	}

	for (auto &texture: textures) {
		_textures[texture.first] = Library::TextureLib->getTexture(texture.second, texture.first);
	}

	_samplerPtr = Library::SamplerLib->getSampler("default");
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
	return _programPtr;
}


std::shared_ptr<Component> Material::clone() const
{
	return std::make_shared<Material>(*this);
}

void Material::bind() {
	//todo bind uniforms
	for (auto &texture: _textures) {
		texture.second->bind();
		texture.second->bindSampler(_samplerPtr);
	}
}

void Material::unbind() {
	//todo unbind uniforms
	for (auto &texture: _textures) {
		texture.second->unbind();
		texture.second->unbindSampler();
	}
}
