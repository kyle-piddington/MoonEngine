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
        std::shared_ptr<GLTexture> LoadTextureFromFile(const std::string & textureName);
    }

}