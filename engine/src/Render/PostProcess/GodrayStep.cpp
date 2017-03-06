// #include "GodrayStep.h"
// #include "Libraries/Library.h"
// #include <glm/gtc/type_ptr.hpp>
// using namespace MoonEngine;

// GodrayStep::GodrayStep(std::string progName):
// _progName(progName),
// _inputTextureStr(inputTextue),
// fboOut(framebuffer)
// {

// }


// void GodrayStep::setup(GLFWwindow * window, Scene * scene)
// {
// 	float _width, _height;
// 	glfwGetWindowSize(window, &_width, &_height);
// 	maskFramebuffer = std::make_shared(_width, _height);
// 	compositionFramebuffer = std::make_shared(_width, _height);
	
// 	_renderProgram = Library::ProgramLib->getProgramForName(_progName);
// 	_inputTexture = Library::TextureLib->getTexture(_inputTextureStr);
// 	sunObject = scene->findGameObjectWithTag(T_SUN);
// 	GLTextureConfiguration colorCFG(width, height, GL_RGBA, GL_RGBA, GL_FLOAT);
// 	_maskTexture = Library::TextureLib->createTexture("_godrayMaskTexture", colorCFG);
// 	maskFramebuffer->addTexture("color",_maskTexture);
// 	compositionFramebuffer->addTexture("composite",Library::TextureLib->getTexture(COMPOSITE_TEXTURE));
// 	_maskProgram = Library::ProgramLib->getProgram("default.program");
// 	_godrayProgram = Library::ProgramLib->getProgram(_progName);
	
// 	_mainCamera = scene->getMainCamera()->getComponent<Camera>();
//     if (_mainCamera == nullptr)
//     {
//         LOG(ERROR, "No Camera in scene!");
//     }
    
// }

// void GodrayStep::drawSun(Scene * scene)
// {
// 	glm::mat4 V = _mainCamera->getView();
// 	glm::mat4 P = _mainCamera->getProjection();
// 	mat = sunObject->getComponent<Material>();
// 	mesh = sunObject->getComponent<Mesh>();
// 	const MeshInfo * meshInfo = mesh->getMesh();
// 	glm::mat4 M = sunObject->getTransform().getMatrix();

// 	//sets the materials geometry shader as active

	
// 	GLProgram * activeProgram = mat->getProgram();
// 	activeProgram->enable();

	
// 	glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
// 	glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
// 	if (activeProgram->hasUniform("iGlobalTime")) {
// 		glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
// 	}

// 	mat->bind();
// 	meshInfo->bind();
	
// 	glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
// 	mesh->draw();
// 	mat->unbind();
// }
// void GodrayStep::render(Scene * scene)
// {
// 	glDisable(GL_DEPTH_TEST);
// 	const auto objects = scene->getLastRenderableObjects();
// 	//Render all 'deferred' objects into the framebuffer as black.
// 	glm::mat4 V = _mainCamera->getView();
// 	glm::mat4 P = _mainCamera->getProjection();
// 	maskFramebuffer->bind(GL_FRAMEBUFFER);
// 	//No depth testing enabled, sun drawn first.
// 	drawSun(scene)
// 	//Draw the rest of the gameObjects
// 	glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
// 	glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
// 	glUniform3f(activeProgram->getUniformLocation("tint"), 0.0, 0.0, 0.0);
// 	for (std::shared_ptr<GameObject> obj : scene->getLastRenderableObjects())
// 	{
// 		mesh = obj->getComponent<Mesh>();
// 		const MeshInfo * meshInfo = mesh->getMesh();
// 		if (mat->isForward()) {
// 			continue;
// 		}
// 		glm::mat4 M = obj->getTransform().getMatrix();
// 		meshInfo->bind();
// 		//No assumptions about the geometry stage is made beyond a P, V, and M Uniforms
// 		glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
// 		mesh->draw();
// 	}
// 	//Enable the composition pass
// 	compositionFramebuffer.bind(GL_FRAMEBUFFER);
// 	//Bind godray prog and calculate screen space position of light.
// 	glm::vec4 screenLightPos = (P * V) * glm::vec4(sunObject->getTransform().getPosition,1);
// 	screenLightPos/=screenLightPos.w;
// 	//Enable blending
// 	glEnable(GL_BLEND);
// 	glBlendEquation(GL_FUNC_ADD);
//     glBlendFunc(GL_ONE, GL_ONE);
//     _godrayProgram->enable();
//     glUniform4fv(_godrayProgram->getUniformLocation("screenLightPos"),1,glm::value_ptr(screenLightPos));
//     maskFramebuffer->getTexture("color").bind(0);
//    	glUniform1i(_godrayProgram->getUniformLocation("occulsionLightTexture"),0);
//     //Blend godrays into composition framebuffer.
//     renderToScreen();
//     glDisable(GL_BLEND);


// }