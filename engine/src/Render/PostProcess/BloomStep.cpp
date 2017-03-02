#include "BasicProgramStep.h"
#include "Libraries/Library.h"
#include "BloomStep.h"

using namespace MoonEngine;

BloomStep::BloomStep()
{

}


void BloomStep::setup(GLFWwindow * window, Scene * scene)
{
	_renderProgram = Library::ProgramLib->getProgramForName(_progName);
	_inputTexture = Library::TextureLib->getTexture(_inputTextureStr);
}

void BloomStep::render(Scene * scene)
{
	if(fboOut != nullptr)
	{
		fboOut->bind(GL_FRAMEBUFFER);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	_renderProgram->enable();
	_inputTexture->bind(0);
	glUniform1i(_renderProgram->getUniformLocation("_inputTexture"),0);
	
	if (_renderProgram->hasUniform("iGlobalTime")) {
		glUniform1f(_renderProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderToScreen();

}