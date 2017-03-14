#include "SimpleTexture.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

SimpleTexture::SimpleTexture(string textureName) :
    Component()
{
    setTexture(textureName);
}

GLTexture * SimpleTexture::getTexture() {
    return _texture;
}

void SimpleTexture::setTexture(string textureName) {
    size_t extPos = textureName.find('.');
    std::string ext = ".png";
    if (extPos != std::string::npos)
    {
        ext = "";
    }
    _texture = Library::TextureLib->createImage(textureName, ext);
}

std::shared_ptr<Component> SimpleTexture::clone() const
{
    return std::make_shared<SimpleTexture>(*this);
}