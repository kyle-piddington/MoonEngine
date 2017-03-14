#pragma once
#include "Component/Component.h"
#include "Component/MeshComponents/Skeleton.h"
#include "Material.h"
#include "Loaders/AssimpModelInfo.h"
#include <glm/glm.hpp>

#include "GLWrapper/GLProgram.h"
#include <GLWrapper/GLSampler.h>
#include <GLWrapper/GLTexture.h>
#include <vector>
/**
 * AssimpMaterial contains the basic rendering routines
 * for rendering an assimp mesh.
 * (Bones etc)
 */
namespace MoonEngine
{
	class AssimpMaterial : public Material
	{
	public:
		AssimpMaterial(std::string program, AssimpModelInfo * modelInfo);
		void start();
		void bind();
		void bindSkeletonBinds(const std::vector<glm::mat4> & bindMatrix);
		void bindSkeletonBones(const std::vector<glm::mat4> & boneMatrix);
		
		void bindTexturesForMesh(int mesh);

		std::shared_ptr<Component> clone() const;
	private:
		Skeleton * _skeleton;
		std::unordered_map<string, Material::texture_unit> loadTextures( unordered_map<string, string> textures);
		std::vector<std::unordered_map<std::string, texture_unit>> meshTextures;

	};
}