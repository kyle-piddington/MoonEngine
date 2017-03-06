#include "SimpleTexture.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

SimpleTexture::SimpleTexture(string textureName) :
    Component()
{
    size_t extPos = textureName.find('.');
    std::string ext = ".png";
    if (extPos != std::string::npos)
    {
        ext = "";
    }
    _texture = Library::TextureLib->createImage(textureName, ext);
}

GLTexture * SimpleTexture::getTexture() {
    return _texture;
}

std::shared_ptr<Component> SimpleTexture::clone() const
{
    return std::make_shared<SimpleTexture>(*this);
}