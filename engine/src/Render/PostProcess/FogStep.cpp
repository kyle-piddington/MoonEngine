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

	_fbo.addTexture(COMPOSITE_TEXTURE, GL_COLOR_ATTACHMENT0);
    _fbo.addTexture(DEPTH_STENCIL_TEXTURE, GL_DEPTH_ATTACHMENT);
    _fbo.addTexture(DEPTH_STENCIL_TEXTURE, GL_STENCIL_ATTACHMENT);
}

void FogStep::render(Scene * scene)
{
	_fbo.bind(GL_FRAMEBUFFER);
	_renderProgram->enable();
    _fbo.getTexture(COMPOSITE_TEXTURE)->bind(0);
    glUniform1i(_renderProgram->getUniformLocation("compositeTexture"), 0);

    _fbo.getTexture(DEPTH_STENCIL_TEXTURE)->bind(1);
    glUniform1i(_renderProgram->getUniformLocation("depthTexture"), 1);

    glDepthMask(GL_FALSE);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawToQuad();

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    _fbo.DBG_DrawToImgui("Sky");
}