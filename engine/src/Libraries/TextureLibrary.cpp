#include <string>
#include "TextureLibrary.h"
#include "Loaders/TextureLoader.h"
#include <memory>
#include "thirdparty/imgui/imgui.h"
#include "Util/Logger.h"

using namespace MoonEngine;

TextureLibrary::TextureLibrary(std::string resourcePath):
        _recPath(resourcePath + "/")
{
    //Add a fallback texture to the library.
    _textures.clear();
    loadDefaultTexture();
}

TextureLibrary::~TextureLibrary()
{
    _textures.clear();
}

/* Get or load a texture */
GLTexture * TextureLibrary::getTexture(std::string textureName, int unit, std::string extension, bool is16f)
{
    if (_textures.find(textureName) == _textures.end())
    {
        LOG(GAME, "Loading texture " + textureName);
        std::shared_ptr<GLTexture> glTexture;
        if(is16f)
        { 
            glTexture = TextureLoader::LoadTextureFromFile16f(unit,
            _recPath + textureName + extension);

        } 
        else{
            glTexture = TextureLoader::LoadTextureFromFile(unit,
            _recPath + textureName + extension);
        }
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

void TextureLibrary::addTexture(std::string textureName, std::shared_ptr<GLTexture> ptr){
    _texturePtrs.push_back(ptr);
    _textures[textureName] = ptr.get();
}


void TextureLibrary::Debug_ShowAllTextures()
{
    //LOG(GAME,"Texture Unit: "  + std::to_string(_texture.second->getTextureId()));
    ImGui::Begin("Textures");
    {
        for (auto _texture : _texturePtrs)
        {
            if (_texture != nullptr)
            {
                ImGui::Image((void *)(uintptr_t) (_texture->getTextureId()), ImVec2(128, 128));
            }

        }
    }
    ImGui::End();
}