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

    _mainCamera = scene->getMainCamera()->getComponent<Camera>();
    if (_mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
}

void GUIStep::render(Scene * scene)
{
	_fbo.bind(GL_FRAMEBUFFER);
	_renderProgram->enable();
	//_compositeTexture->bind(0);

	glm::mat4 V = _mainCamera->getView();
	glm::mat4 P = glm::ortho(0.0f, (float)_width, (float)_height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(_renderProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
	glUniformMatrix4fv(_renderProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

//	glEnable(GL_BLEND);
//	glBlendEquation(GL_FUNC_ADD);
//	glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

	GLTexture * sample = Library::TextureLib->createImage("solid_white", ".png");
	sample->bind(0);
	glUniform1i(_renderProgram->getUniformLocation("_guiTexture"), 0);


	for (std::shared_ptr<GameObject> obj : scene->getGuiGameObjects())
	{
        glm::mat4 M = obj->getTransform().getMatrix();
        glUniformMatrix4fv(_renderProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));


        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawToQuad();
        /*mat = obj->getComponent<Material>();
        mesh = obj->getComponent<Mesh>();
        meshInfo = mesh->getMesh();

        glm::mat4 M = obj->getTransform().getMatrix();


        mat->bind();
        meshInfo->bind();

        glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

        mesh->draw();
        mat->unbind();*/
	}

	//Disable the filled depth buffer
//	glDisable(GL_BLEND);
    _fbo.DBG_DrawToImgui("GUI");
}