#include <string>
#include "TextureLibrary.h"
#include "Loaders/TextureLoader.h"
#include <memory>
#include "thirdparty/imgui/imgui.h"
using namespace MoonEngine;

TextureLibrary::TextureLibrary(std::string resourcePath):
        _recPath(resourcePath + "/") {
    //Add a fallback texture to the library.
    _textures.clear();
    loadDefaultTexture();

}

TextureLibrary::~TextureLibrary()
{
    _textures.clear();
}

/* Get or load a texture. Do not include an image extension */
GLTexture * TextureLibrary::getTexture(std::string textureName, int unit, std::string extension)
{
    if (_textures.find(textureName) == _textures.end())
    {
        std::shared_ptr<GLTexture> glTexture = TextureLoader::LoadTextureFromFile(unit, _recPath + textureName + extension);
        _texturePtrs.push_back(glTexture);
        if (glTexture != nullptr)
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
    std::shared_ptr<GLTexture> glTexture = TextureLoader::LoadTextureFromFile(0, _recPath + "default.png");
    
    _textures["default"] = glTexture.get();
}

void TextureLibrary::Debug_ShowAllTextures()
{
    //LOG(GAME,"Texture Unit: "  + std::to_string(_texture.second->getTextureId()));
    ImGui::Begin("Textures");
    {
        for(auto _texture : _texturePtrs)
        {
            if(_texture != nullptr)
            {
                ImGui::Image((void*)(_texture->getTextureId()),ImVec2(128,128));                
            }

        }
    }
    ImGui::End();
}