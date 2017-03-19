#include "HDRStep.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

HDRStep::HDRStep(std::string progName):
	_progName(progName),
	_exposure(1.0)
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

	;

	ImGui::Begin("Bright");
	ImGui::DragFloat("Exposure", &_exposure, -1.0f, 3.0f);
	ImGui::End();

    /* Start below one, as night falls increase */
    float increasingExposure = max(-0.2f, scene->getGlobalTime() - 0.5f);

    if (_renderProgram->hasUniform("exposure")) {
        glUniform1f(_renderProgram->getUniformLocation("exposure"), _exposure + increasingExposure);
    }
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawToQuad();
}