#pragma once

#include "Component/Component.h"
#include <GLWrapper/GLTexture.h>
/**
 * Super simple just holds a texture
 */

namespace MoonEngine
{
    class SimpleTexture: public Component
    {
    public:
        SimpleTexture(string textureName);

        GLTexture * getTexture();
        virtual std::shared_ptr<Component> clone() const;

    private:
        GLTexture * _texture;

    };
}