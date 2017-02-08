#include "Material.h"
#include "Libraries/Library.h"
#include <iostream>

using namespace MoonEngine;

Material::Material(glm::vec3 tint, vector<string> programNames, unordered_map<string, string> textures, bool forward) :
    Component(),
    _activeProgram(0),
    _programs(std::vector<GLProgram *>()),
    _tint(tint),
    _textures(std::unordered_map<string, texture_unit>()),
    _texture_unit(0),
	_forward(forward)
{
    _programs.reserve(programNames.size());
    for (int i = 0; i < programNames.size(); i++)
    {
        _programs.push_back( Library::ProgramLib->getProgramForName(programNames[i]));
        if (_programs.at(i) == nullptr)
        {
            _programs.push_back(Library::ProgramLib->getProgramForName("default.program"));
        }
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
        texture_unit textureUnit = {Library::TextureLib->getTexture(texture.second, ext), _texture_unit++};
        _textures[texture.first] = textureUnit;
    }

    _sampler = Library::SamplerLib->getSampler("default");
}

Material::Material(glm::vec3 tint, std::string programName, unordered_map<string, string> textures, bool forward):
    Component(),
    _activeProgram(0),
    _programs(std::vector<GLProgram *>()),
    _tint(tint),
    _textures(std::unordered_map<string, texture_unit>()),
    _texture_unit(0),
    _forward(forward)
{
    vector<string> programNames;
    if (!forward) {
        /* If deferred and only one shader provided, use default geom */
        programNames.push_back("geom.program");
    }
    programNames.push_back(programName);

    *this = Material::Material(tint, programNames, textures, forward);
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
    return _programs[_activeProgram];
}

void Material::setActiveProgram(int program)
{
    _activeProgram = program;
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
        glBindSampler(texture.unit, _sampler->getId());
        glUniform1i(_programs[_activeProgram]->getUniformLocation(_texture.first), texture.unit);
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