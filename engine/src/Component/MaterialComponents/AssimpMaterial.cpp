#include "AssimpMaterial.h"
#include "Libraries/Library.h"
#include <iostream>


std::unordered_map<string, Material::texture_unit> AssimpMaterial::loadTextures( unordered_map<string, string> textures)
{
   std::unordered_map<string, Material::texture_unit> txMap;
   for (auto & texture: textures)
   {
        size_t extPos = texture.second.find('.');
        std::string ext = ".png";
        if (extPos != std::string::npos)
        {
            ext = "";
        }
            // uniform name <=> texture
        Material::texture_unit textureUnit = {Library::TextureLib->getTexture(texture.second, ext), _texture_unit++};
        txMap[texture.first] = textureUnit;
    }
    
    return txMap;  
}

AssimpMaterial::AssimpMaterial(std::string program, AssimpModelInfo * modelInfo):
	Material(program, false)
{
	for(auto info : modelInfo->getMeshInfo())
	{
		meshTextures.push_back(loadTextures(info.textures));
	}
	_sampler = Library::SamplerLib->getSampler("default"); 
}



void AssimpMaterial::bind()
{

	//Specific binds are managed by a mesh. (Since assimp models are drawn) mesh by mesh
}

void AssimpMaterial::bindTexturesForMesh(int mesh)
{
	for (auto & _texture: meshTextures[mesh])
    {
        Material::texture_unit texture = _texture.second;
        texture.gl_texture->bind(texture.unit);
        glUniform1i(_program->getUniformLocation(_texture.first),texture.unit);
        LOG_GL(__FILE__, __LINE__);
    }

}

std::shared_ptr<Component> AssimpMaterial::clone() const
{
    return std::make_shared<Material>(*this);
}
