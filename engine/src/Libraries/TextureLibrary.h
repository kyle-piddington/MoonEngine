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
        /**
         * Create an image, or return a texture if it's already
         * in the map
         * @param  textureName name of the texture
         * @param  extension  extension of the texture
         * @param  f16         Whether or not to lad a  16 bit texture
         * @return            Poitner to the texture.
         */
        GLTexture * createImage(std::string textureName, std::string extension = ".png", bool f16 = false);

        GLTexture * getTexture(std::string textureName);

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