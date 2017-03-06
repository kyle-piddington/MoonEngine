#include "GBuffer.h"

using namespace MoonEngine;

MoonEngine::GBuffer::GBuffer(int width, int height) :
GLFramebuffer(width, height)
{
}

void GBuffer::bindForGeomPass() {
    drawColorAttachments(_colorCount);
}

void GBuffer::bindForStencilPass() {
    glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass() {
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    for (auto &tex : _textureHandles) {
        if (tex.first != COMPOSITE_TEXTURE && tex.first != "depth") {
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

