#include "FogStep.h"
#include "Libraries/Library.h"
#include "GlobalFuncs/GlobalFuncs.h"

using namespace MoonEngine;

FogStep::FogStep(int width, int height):
    _fbo(width, height),
    _width(width),
    _height(height)
{
}


void FogStep::setup(GLFWwindow * window, Scene * scene)
{
	_renderProgram = Library::ProgramLib->getProgramForName("postprocess/post_fog.program");

    _colorTexture = Library::TextureLib->getTexture("skycolor");

    _fbo.addTexture(COMPOSITE_TEXTURE, GL_COLOR_ATTACHMENT0);
    _fbo.addTexture(DEPTH_STENCIL_TEXTURE, GL_DEPTH_ATTACHMENT);
    _fbo.addTexture(DEPTH_STENCIL_TEXTURE, GL_STENCIL_ATTACHMENT);
}

void FogStep::render(Scene * scene)
{
	_fbo.bind(GL_FRAMEBUFFER);
	_renderProgram->enable();

    glDepthMask(GL_FALSE);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    _colorTexture->bind(0);
    glUniform1i(_renderProgram->getUniformLocation("colorTexture"), 0);

    glUniform1f(_renderProgram->getUniformLocation("iGlobalTime"),std::min(0.95f, scene->getGlobalTime()));

    glClear(GL_COLOR_BUFFER_BIT);

    drawToQuad();
    glDisable(GL_BLEND);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    _fbo.DBG_DrawToImgui("Fog");
}