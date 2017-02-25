#include "Material.h"
#include "Libraries/Library.h"
#include <iostream>

using namespace MoonEngine;

Material::Material(glm::vec3 tint, string programName, unordered_map<string, string> textures, bool forward) :
    Component(),
    _tint(tint),
    _textures(std::unordered_map<string, texture_unit>()),
    _texture_unit(0),
	_forward(forward)
{
    _program = Library::ProgramLib->getProgramForName(programName);
    if (_program == nullptr)
    {
        _program = Library::ProgramLib->getProgramForName("default.program");
    }

    for (auto & texture: textures)
    {
        size_t extPos = texture.second.find('.');
        std::string ext = ".png";
        if (extPos != std::string::npos)
        {
            ext = "";
        }
        // uniform name <=> texture
        texture_unit textureUnit = {Library::TextureLib->createImage(texture.second, ext), _texture_unit++};
        _textures[texture.first] = textureUnit;
    }

    _sampler = Library::SamplerLib->getSampler("default");
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
    return _program;
}

std::shared_ptr<Component> Material::clone() const
{
    return std::make_shared<Material>(*this);
}

void Material::bind()
{
    for (auto & _texture: _textures)
    {
        texture_unit texture = _texture.second;
        texture.gl_texture->bind(texture.unit);
        glUniform1i(_program->getUniformLocation(_texture.first),texture.unit);
        LOG_GL(__FILE__, __LINE__);
    }
}

void Material::unbind()
{
    //todo unbind uniforms
    for (auto & _texture: _textures)
    {
        texture_unit texture = _texture.second;
        texture.gl_texture->unbind(texture.unit);
        glBindSampler(texture.unit, 0);
    }

}


void Material::addTexture(std::string uniformName, GLTexture * texture)
{
    _textures[uniformName] = {texture, _texture_unit++};
}