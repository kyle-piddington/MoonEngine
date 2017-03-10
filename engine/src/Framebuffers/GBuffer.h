#pragma once
#include "GLWrapper/GLFramebuffer.h"
namespace MoonEngine
{

    class GBuffer : public GLFramebuffer
    {
    public:

        GBuffer(int width, int height);
        void addTexture(const std::string & textureName, GLenum attachmentInfo);
        void addTexture(const std::string & textureName, GLenum attachmentInfo, vector<TexParameter> texParameters);
        void bindForGeomPass();
        void bindForStencilPass();
        void bindForLightPass();
        void bindForOutput();
    private:
        int _colorCount;
    };

}