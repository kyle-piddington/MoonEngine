#include "AssimpMaterial.h"
#include "Libraries/Library.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


std::unordered_map<string, Material::texture_unit> AssimpMaterial::loadTextures( unordered_map<string, string> textures)
{
   _program->enable();
   std::unordered_map<string, Material::texture_unit> txMap;
   for (auto & texture: textures)
   {
        size_t extPos = texture.second.find('.');
        std::string ext = ".png";
        if (extPos != std::string::npos)
        {
            ext = "";
        }
        if(_program->hasUniform(texture.first))
        {

            Material::texture_unit textureUnit = {Library::TextureLib->createImage(texture.second, ext), _texture_unit++};
            txMap[texture.first] = textureUnit;
            // uniform name <=> texture
        }

    }
    
    return txMap;  
}

AssimpMaterial::AssimpMaterial(std::string program, AssimpModelInfo * modelInfo , std::string shadowProgram) :
	Material(program, false),
    meshTextures(),
    _skeleton(nullptr)
{
	for(auto info : modelInfo->getMeshInfo())
	{
		meshTextures.push_back(loadTextures(info.textures));
	}
	_sampler = Library::SamplerLib->getSampler("default"); 
	_skinnedShadowProgram = Library::ProgramLib->getProgramForName(shadowProgram);
}


bool AssimpMaterial::hasShadowProgram()
{
	return _skeleton != nullptr;
}

GLProgram * AssimpMaterial::getShadowProgram() const
{
	return _skinnedShadowProgram;
}

void AssimpMaterial::bindForShadow()
{
	if (_skeleton != nullptr)
	{
		_skeleton->finalizeAnimation();
		std::vector<Bone> bones = _skeleton->getAllBones();
		std::vector<glm::mat4> boneTransforms(bones.size());
		for (int i = 0; i < bones.size(); i++)
		{
			boneTransforms[bones[i].getIndex()] = bones[i].getAnimMatrix();
		}
		bindSkeletonBones(boneTransforms, _skinnedShadowProgram);
	}
}
void AssimpMaterial::bind()
{
    if(_skeleton != nullptr)
    {
        _skeleton->finalizeAnimation();
        std::vector<Bone> bones = _skeleton->getAllBones();
        std::vector<glm::mat4> boneTransforms(bones.size());
        for(int i = 0; i < bones.size(); i++)
        {
            boneTransforms[bones[i].getIndex()] = bones[i].getAnimMatrix();
        }
        bindSkeletonBones(boneTransforms, _program);
    }
	//Specific binds are managed by a mesh. (Since assimp models are drawn) mesh by mesh
   
}

void AssimpMaterial::start()
{
    //Try and get skeleton
    _skeleton = getGameObject()->getComponent<Skeleton>();
}

void AssimpMaterial::bindTexturesForMesh(int mesh)
{
    LOG_GL(__FILE__, __LINE__);
	for (auto & _texture: meshTextures[mesh])
    {
        Material::texture_unit texture = _texture.second;
        texture.gl_texture->bind(texture.unit);
        LOG_GL(__FILE__, __LINE__);
        glUniform1i(_program->getUniformLocation(_texture.first),texture.unit);
        LOG_GL(__FILE__, __LINE__);
    }

}

void AssimpMaterial::bindSkeletonBinds(const std::vector<glm::mat4> &bindMatrix, GLProgram * program)
{
    for(int i = 0; i < bindMatrix.size(); i++)
    {
        glm::mat4 m = bindMatrix[i];
        glUniformMatrix4fv(program->getUniformLocation("gBinds[" + std::to_string(i) + "]"),1,GL_FALSE,glm::value_ptr(m));    
    }
    
    LOG_GL(__FILE__, __LINE__);
}
void AssimpMaterial::bindSkeletonBones(const std::vector<glm::mat4> &boneMatrix, GLProgram * program)
{
    for(int i = 0; i < boneMatrix.size(); i++)
    {
        glm::mat4 m = boneMatrix[i];
        glUniformMatrix4fv(program->getUniformLocation("gBones[" + std::to_string(i) + "]"),1,GL_FALSE,glm::value_ptr(m));    
    } 
    LOG_GL(__FILE__, __LINE__);
}

std::shared_ptr<Component> AssimpMaterial::clone() const
{
    return std::make_shared<Material>(*this);
}
