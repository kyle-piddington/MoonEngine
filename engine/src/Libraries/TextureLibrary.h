#pragma once
/**
 * TextureLibrary provides a mapping of
 * texture names to textures
 */
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "GLWrapper/GLTexture.h"

namespace MoonEngine
{
    class TextureLibrary
    {
    public:
        TextureLibrary(std::string resourcePath);

        ~TextureLibrary();

        GLTexture * getTexture(std::string textureName, std::string extension = ".png", bool f16 = false);
        //Track a texture in the library.
        void addTexture(std::string textureName, std::shared_ptr<GLTexture> ptr);
   
        void Debug_ShowAllTextures();

    private:
        std::string _recPath;

        void loadDefaultTexture();

        std::unordered_map<std::string, GLTexture *> _textures;
        std::vector<std::shared_ptr<GLTexture>> _texturePtrs;
    };
}