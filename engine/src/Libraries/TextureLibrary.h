#pragma once
/**
 * TextureLibrary provides a mapping of
 * texture names to textures
 */
#include <string>
#include <unordered_map>
#include <vector>
#include "GL/GLTexture.h"


namespace MoonEngine
{

    class TextureLibrary
    {
    public:
        TextureLibrary(std::string resourcePath);
        ~TextureLibrary();

        GLTexture * getTexture(std::string textureName, int unit);

    private:
        std::string _recPath;
        void loadDefaultTexture();
        std::unordered_map<std::string, GLTexture *> _textures;
        std::vector< std::shared_ptr<GLTexture> > _texturePtrs;
    };
}