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
    _glowFramebuffer(width, height),
    _tempFramebuffer(width, height),
    _width(width),
    _height(height)
{

}


void configureMipmapTexture(GLTexture * texture)
{
	texture->bindRaw();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
}
void BloomStep::setup(GLFWwindow * window, Scene * scene)
{
    string path = "postprocess/bloom/post_bloom_";
    _glowProgram = Library::ProgramLib->getProgramForName(path + "glow.program");
    _blurProgram = Library::ProgramLib->getProgramForName(path + "blur.program");

	_compositeTexture = Library::TextureLib->getTexture(COMPOSITE_TEXTURE);

    GLTextureConfiguration colorCFG(_width, _height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    _glowTexture = Library::TextureLib->createTexture("_bloomGlowTexture", colorCFG);
    _tempTexture = Library::TextureLib->createTexture("_bloomTempTexture", colorCFG);

    _compositeFramebuffer.addTexture("composite", *_compositeTexture, GL_COLOR_ATTACHMENT0);
	_compositeFramebuffer.addDepthRenderbuffer();
    _glowFramebuffer.addTexture("glow", *_glowTexture, GL_COLOR_ATTACHMENT0);
	configureMipmapTexture(_glowTexture);
    _tempFramebuffer.addTexture("temp", *_tempTexture, GL_COLOR_ATTACHMENT0);
	_tempFramebuffer.addDepthRenderbuffer();
	
}

void BloomStep::extractGlow()
{
    _glowFramebuffer.bind(GL_FRAMEBUFFER);
	
    _glowProgram->enable();
    _compositeTexture->bind(0);
    glUniform1i(_glowProgram->getUniformLocation("compositeTexture"), 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawToQuad();
	_glowTexture->bindRaw();
	glGenerateMipmap(GL_TEXTURE_2D);
	LOG_GL(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void BloomStep::blurPass()
{
    _blurProgram->enable();
    /* Add each pass to composite texture */
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
	LOG_GL(__FILE__, __LINE__);


    // For each level of detail
    for (int lod = 0; lod <= 2; lod++) {
        glUniform1f(_blurProgram->getUniformLocation("lod"), (float)lod);
		LOG_GL(__FILE__, __LINE__);

        // horizontal blur
        _tempFramebuffer.bind(GL_FRAMEBUFFER);
		LOG_GL(__FILE__, __LINE__);
        _glowTexture->bind(0);
        glUniform1i(_blurProgram->getUniformLocation("glowTexture"), 0);
        glUniform2f(_blurProgram->getUniformLocation("offset"), 1.2f/ _width, 0);
		LOG_GL(__FILE__, __LINE__);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawToQuad();
		LOG_GL(__FILE__, __LINE__);
        //vertical blur
        _compositeFramebuffer.bind(GL_FRAMEBUFFER);
        _tempTexture->bind(0);
        glUniform2f(_blurProgram->getUniformLocation("offset"), 0, 1.2f/ _height);

        drawToQuad();
		LOG_GL(__FILE__, __LINE__);
    }

    glDisable(GL_BLEND);
}

void BloomStep::render(Scene * scene)
{
    extractGlow();
    blurPass();

    _glowFramebuffer.DBG_DrawToImgui("Bloom");
    _tempFramebuffer.DBG_DrawToImgui("Bloom");
    _compositeFramebuffer.DBG_DrawToImgui("Bloom");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}