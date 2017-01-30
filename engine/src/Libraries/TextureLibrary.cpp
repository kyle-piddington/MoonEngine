#include <string>
#include "TextureLibrary.h"
#include <memory>
using namespace MoonEngine;

TextureLibrary::TextureLibrary(std::string resourcePath):
        _recPath(resourcePath + "/") {
    //Add a fallback texture to the library.
    loadDefaultTexture();
}

TextureLibrary::~TextureLibrary()
{
    _textures.clear();
}

/* Get or load a texture. Do not include an image extension */
GLTexture * TextureLibrary::getTexture(std::string textureName, MaterialProperty materialProperty)
{
    if (_textures.find(textureName) == _textures.end())
    {
        std::shared_ptr<GLTexture> glTexture = std::make_shared<GLTexture>(materialProperty);
        if (glTexture->init(textureName))
        {
            _textures[textureName] = glTexture.get();
        }
        else
        {
            _textures[textureName] = _textures["default"];
        }
    }
    return _textures[textureName];
}

void TextureLibrary::loadDefaultTexture()
{
    std::shared_ptr<GLTexture> glTexture = std::make_shared<GLTexture>(GL_TEXTURE0);
    glTexture->init("default");
    _textures["default"] = glTexture.get();
}