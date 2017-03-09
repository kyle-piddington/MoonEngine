#include "GBuffer.h"

using namespace MoonEngine;

MoonEngine::GBuffer::GBuffer(int width, int height) :
GLFramebuffer(width, height), _colorCount(0)
{
}

void MoonEngine::GBuffer::addTexture(const std::string & textureName, GLenum attachmentInfo) 
{
    GLFramebuffer::addTexture(textureName, attachmentInfo);
    if (attachmentInfo >= GL_COLOR_ATTACHMENT0 && attachmentInfo < GL_COLOR_ATTACHMENT15) 
        _colorCount += 1;
}

void MoonEngine::GBuffer::addTexture(const std::string & textureName, GLenum attachmentInfo, vector<TexParameter> texParameters)
{
    GLFramebuffer::addTexture(textureName, attachmentInfo, texParameters);
    if (attachmentInfo >= GL_COLOR_ATTACHMENT0 && attachmentInfo < GL_COLOR_ATTACHMENT15)
        _colorCount += 1;
}

void GBuffer::bindForGeomPass() {
    vector<GLuint> colors;
    for (int i = 0; i < _colorCount; i++)
        colors.push_back(GL_COLOR_ATTACHMENT0 + i);
    bind(GL_DRAW_FRAMEBUFFER);
    glDrawBuffers(_colorCount, &colors[0]);
}

void GBuffer::bindForStencilPass() {
    glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass() {
    glDrawBuffer(COMPOSITE_ATTACHMENT);
    for (auto &tex : _textureHandles) {
        if (tex.first != COMPOSITE_TEXTURE && tex.first != DEPTH_STENCIL_TEXTURE) {
            glActiveTexture(GL_TEXTURE0 + tex.second.unit);
            glBindTexture(GL_TEXTURE_2D, tex.second.gl_texture->getTextureId());
        }
    }
}

void GBuffer::bindForOutput() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    bind(GL_READ_FRAMEBUFFER);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}



