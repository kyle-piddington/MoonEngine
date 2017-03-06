#pragma once
#include "GLWrapper/GLFramebuffer.h"
namespace MoonEngine
{

    class GBuffer : public GLFramebuffer
    {
    public:

        GBuffer(int width, int height);
        void bindForGeomPass();
        void bindForStencilPass();
        void bindForLightPass();
        void bindForOutput();
    };

}