#include "GUIStep.h"
#include "Libraries/Library.h"

using namespace MoonEngine;

GUIStep::GUIStep(int width, int height):
    _fbo(width, height),
    _width(width),
    _height(height)
{
}


void GUIStep::setup(GLFWwindow * window, Scene * scene)
{
	_renderProgram = Library::ProgramLib->getProgramForName("postprocess/gui.program");
	_compositeTexture = Library::TextureLib->getTexture(COMPOSITE_TEXTURE);

	_fbo.addTexture("composite", *_compositeTexture, GL_COLOR_ATTACHMENT0);
    _fbo.addDepthRenderbuffer();
}

void GUIStep::render(Scene * scene)
{
	_fbo.bind(GL_FRAMEBUFFER);
	_renderProgram->enable();

	glm::mat4 P = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(_renderProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

	for (std::shared_ptr<GameObject> obj : scene->getGuiGameObjects())
	{
        obj->getComponent<SimpleTexture>()->getTexture()->bind(0);
        glUniform1i(_renderProgram->getUniformLocation("_guiTexture"), 0);

        glm::mat4 M = obj->getTransform().getMatrix();
        glUniformMatrix4fv(_renderProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

        drawToQuad();
	}

    _fbo.DBG_DrawToImgui("GUI");
}