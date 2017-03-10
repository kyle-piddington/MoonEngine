#include "BloomStep.h"
#include "Libraries/Library.h"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "GLWrapper/GLConstants.h"

using namespace MoonEngine;

BloomStep::BloomStep(int width, int height):
    _compositeFramebuffer(width, height),
    _glowFramebuffer(width / 2, height / 2),
    _tempFramebuffer(width / 2, height / 2),
    _width(width),
    _height(height)
{

}


void BloomStep::setup(GLFWwindow * window, Scene * scene)
{
    string path = "postprocess/bloom/post_bloom_";
    _glowProgram = Library::ProgramLib->getProgramForName(path + "glow.program");
    _blurProgram = Library::ProgramLib->getProgramForName(path + "blur.program");

	_compositeTexture = Library::TextureLib->getTexture(COMPOSITE_TEXTURE);

    GLTextureConfiguration colorCFG(_width / 2, _height / 2, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    _glowTexture = Library::TextureLib->createTexture(BLOOM_GLOW_TEXTURE, colorCFG);
    _tempTexture = Library::TextureLib->createTexture(BLOOM_TEMP_TEXTURE, colorCFG);

    _compositeFramebuffer.addTexture(COMPOSITE_TEXTURE, GL_COLOR_ATTACHMENT0);
    _glowFramebuffer.addTexture(BLOOM_GLOW_TEXTURE, GL_COLOR_ATTACHMENT0);
    _tempFramebuffer.addTexture(BLOOM_TEMP_TEXTURE, GL_COLOR_ATTACHMENT0);
}

void BloomStep::extractGlow()
{
    _glowFramebuffer.bind(GL_FRAMEBUFFER);
    _glowProgram->enable();
    _compositeTexture->bind(0);
    glUniform1i(_glowProgram->getUniformLocation("compositeTexture"), 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawToQuad();
}

void BloomStep::blurPass()
{
//    _compositeFramebuffer.bind(GL_FRAMEBUFFER);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _blurProgram->enable();
    glUniform1i(_blurProgram->getUniformLocation("glowTexture"), 0);

    glDisable(GL_DEPTH_TEST);

    // Repeat blur 5 times
    int repeat = 5;
    for (int i = 0; i < repeat; i++) {
        // horizontal blur
        _tempFramebuffer.bind(GL_FRAMEBUFFER);

        _glowTexture->bind(0);
        glUniform2f(_blurProgram->getUniformLocation("offset"), 1.2f / _width, 0);

        drawToQuad();

        //vertical blur
        _glowFramebuffer.bind(GL_FRAMEBUFFER);

        if (i == repeat - 1) {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);

            glViewport(0,0,_width,_height);
            _compositeFramebuffer.bind(GL_FRAMEBUFFER);
        }

        _tempTexture->bind(0);
        glUniform2f(_blurProgram->getUniformLocation("offset"), 0, 1.2f / _height);

        drawToQuad();
    }
    glDisable(GL_BLEND);
}

void BloomStep::render(Scene * scene)
{
    glViewport(0, 0, _glowFramebuffer.getWidth(), _glowFramebuffer.getHeight());
    extractGlow();
    blurPass();

    _glowFramebuffer.DBG_DrawToImgui("Bloom");
    _tempFramebuffer.DBG_DrawToImgui("Bloom");
    _compositeFramebuffer.DBG_DrawToImgui("Bloom");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
