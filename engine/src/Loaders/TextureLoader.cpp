#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb_image.h>
#include <iostream>
#include <memory>
#include "Util/Logger.h"
#include "GLWrapper/GLTextureConfiguration.h"

using namespace MoonEngine;

std::shared_ptr<GLTexture> TextureLoader::LoadTextureFromFile(GLuint unit, const std::string & textureName)
{
	int width = 0, height = 0, ncomps = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(textureName.c_str(), &width, &height, &ncomps, 0);
    /* File not found */
    if (!data) {
       	LOG(ERROR, "Could not load texture from file named "  + textureName);
        return nullptr;
    }

    GLenum texType;
    switch(ncomps){
    	case 1:
    		texType = GL_RED;
    	case 2:
    		texType = GL_RG;
    	case 3:
    		texType = GL_RGB;
    	case 4:
    		texType = GL_RGBA;
    	default:
    		texType = GL_RGB;
    }

    GLTextureConfiguration cfg(width, height, texType, texType, GL_UNSIGNED_BYTE);
    auto tex = std::make_shared<GLTexture>(unit);
    //If texture fails to initialize...
    if(!tex->init(data, cfg))
    {
    	return nullptr;
    }
    stbi_image_free(data);
    return tex;

}