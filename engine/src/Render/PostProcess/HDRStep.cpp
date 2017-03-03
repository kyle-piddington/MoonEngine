#include "HDRStep.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

HDRStep::HDRStep(std::string progName, int width, int height):
	_progName(progName),
	fboOut(width, height)
{

}

void HDRStep::setup(GLFWwindow * window, Scene * scene)
{
	_renderProgram = Library::ProgramLib->getProgramForName(_progName);
	_inputTexture = Library::TextureLib->getTexture(COMPOSITE_TEXTURE);

}

void HDRStep::render(Scene * scene)
{
	
	fboOut.bind(GL_FRAMEBUFFER);
	_renderProgram->enable();
	_inputTexture->bind(0);
	glUniform1i(_renderProgram->getUniformLocation("_inputTexture"), 0);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawToQuad();

}