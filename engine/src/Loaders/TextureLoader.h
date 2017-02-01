#pragma once
#include "GLWrapper/GLTexture.h"

/**
 * Load a texture from a file
 * and into a texture.
 */

namespace MoonEngine
{
	namespace TextureLoader
	{
		std::shared_ptr<GLTexture> LoadTextureFromFile(GLuint handle, const std::string & textureName);		
	}

}