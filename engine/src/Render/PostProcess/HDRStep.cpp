#include "HDRStep.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

HDRStep::HDRStep(std::string progName):
	_progName(progName)
{

}

void HDRStep::setup(GLFWwindow * window, Scene * scene)
{
	_renderProgram = Library::ProgramLib->getProgramForName(_progName);
	_inputTexture = Library::TextureLib->getTexture(COMPOSITE_TEXTURE);

}

void HDRStep::render(Scene * scene)
{

    /* Bind default framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_renderProgram->enable();
	_inputTexture->bind(0);
	glUniform1i(_renderProgram->getUniformLocation("hdrTexture"), 0);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawToQuad();
}