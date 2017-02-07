#pragma once

#include "GLWrapper/GLTexture.h"
#include <memory>
/**
 * Load a texture from a file
 * and into a texture.
 */

namespace MoonEngine
{
    namespace TextureLoader
    {
        std::shared_ptr<GLTexture> LoadTextureFromFile(GLuint handle, const std::string & textureName);

        /**
         * Load a texture into a buffer
         * @param  bfr         buffer to load data into. 
         * @param  textureName name of the texture
         * @return             number of components. 
         * @note: Call stbi_image_free to clean up
         */
     	int LoadTextureToBuffer(unsigned char ** bfrPtr, const std::string & textureName, int * width = nullptr, int * height = nullptr);

           
    }

}