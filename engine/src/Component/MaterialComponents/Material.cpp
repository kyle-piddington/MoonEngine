#include "Material.h"
#include "Libraries/Library.h"
#include "thirdparty/imgui/imgui.h"
#include <string>

using namespace MoonEngine;

Material::Material(glm::vec3 tint, vector<string> programNames, unordered_map<string, string> textures, bool forward) :
    Component(),
    _textures(std::unordered_map<string, GLTexture *>()),
    _tint(tint),
    _activeProgram(0),
    _texture_unit(0),
	_forward(forward)
{

    int i = 0;
    for (i = 0; i < programNames.size(); i++)
    {
        _programs[i] = Library::ProgramLib->getProgramForName(programNames[i]);
        if (_programs[i] == nullptr)
        {
            _programs[i] = Library::ProgramLib->getProgramForName("default.program");
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
        _textures[texture.first] = Library::TextureLib->getTexture(texture.second, _texture_unit++, ext);
    }

    _samplerPtr = Library::SamplerLib->getSampler("default");
}

Material::Material(glm::vec3 tint, std::string programName, unordered_map<string, string> textures, bool forward)
{
    vector<string> programNames = {programName};
    Material::Material(tint, programNames, textures, forward);
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
        _texture.second->bind();
        _texture.second->bindSampler(_samplerPtr);
        glUniform1i(_programs[_activeProgram]->getUniformLocation(_texture.first), _texture.second->getUnit());
    }
}

void Material::unbind()
{
    //todo unbind uniforms
    for (auto & texture: _textures)
    {
        texture.second->unbind();
        texture.second->unbindSampler();
    }
}