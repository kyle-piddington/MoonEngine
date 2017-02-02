#include "ProgramRenderer.h"
#include "Util/Logger.h"
#include "GLWrapper/OpenGL.h"
#include <glm/gtc/type_ptr.hpp>
#include "Component/Components.h"
#include "GameObject/GameObject.h"
#include "Geometry/MeshCreator.h"
#include "thirdparty/imgui/imgui.h"
#include <iostream>
using namespace MoonEngine;



ProgramRenderer::ProgramRenderer():
	mainCamera(nullptr),
	//renderToFB(800,600),
	framebufferColorTexture(0),
	framebufferDepthStencilTexture(1)
{
	// renderQuad = MeshCreator::CreateQuad(glm::vec2(-1,1), glm::vec2(1,1));
	// assert(framebufferColorTexture.init(GLTextureConfiguration(800,600,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE)));
	// assert(framebufferDepthStencilTexture.init(GLTextureConfiguration(800,600,GL_DEPTH24_STENCIL8,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8)));
	// renderToFB.addTexture("color",framebufferColorTexture,GL_COLOR_ATTACHMENT0);
	// renderToFB.addTexture("depthStencil",framebufferDepthStencilTexture,GL_DEPTH_STENCIL_ATTACHMENT);

}

void ProgramRenderer::setup(Scene * scene)
{
	mainCamera = scene->findGameObjectWithComponent<Camera>()->getComponent<Camera>();
	if(mainCamera == nullptr)
	{
		LOG(ERROR , "No Camera in scene!");
	}
	//Swing through all rendering components and load their programs.

	glClearColor(0.2f,0.2f,0.6f,1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ProgramRenderer::render(Scene * scene)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	//Bind program
	
	glm::mat4 V =  mainCamera->getView();
	glm::mat4 P = mainCamera->getProjection();
	//No binning
	//World position light
	//(Hardcoded for now)
	glm::vec3 lightDir(1,1,1);
	GLProgram * activeProgram = nullptr;
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	for(std::shared_ptr<GameObject> obj : scene->getRenderableGameObjects())
	{
		glm::mat4 M = obj->getTransform().getMatrix();
		glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V*M)));
		Material * mat = obj->getComponent<Material>();

		if(activeProgram != mat->getProgram())
		{
			activeProgram = mat->getProgram();
			activeProgram->enable();
			glUniformMatrix4fv(
			activeProgram->getUniformLocation("P"),1,GL_FALSE, glm::value_ptr(P));
		
			glUniformMatrix4fv(
			activeProgram->getUniformLocation("V"),1,GL_FALSE, glm::value_ptr(V));
			if(activeProgram->hasUniform("lightDirWorld"))
			{
				glUniform3f(activeProgram->getUniformLocation("lightDirWorld"),lightDir.x,lightDir.y,lightDir.z);
			}
		}

		glm::vec3 tint = mat->getTint();
		const MeshInfo * mesh = obj->getComponent<Mesh>()->getMesh();
		mesh->bind();
		//@TODO: Refactor these later
		glUniform3f(activeProgram->getUniformLocation("tint"),tint.x,tint.y,tint.z);
		mat->bind();
		glUniformMatrix4fv(
			activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix3fv(
			activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
		if (obj->getComponent<InstanceMesh>() != nullptr) {
			glDrawElementsInstanced(GL_TRIANGLES, mesh->numTris, GL_UNSIGNED_SHORT, mesh->indexDataOffset, obj->getComponent<InstanceMesh>()->_numOfInstances);
		}
		else {
			glDrawElementsBaseVertex(GL_TRIANGLES,
				mesh->numTris,
				GL_UNSIGNED_SHORT,
				mesh->indexDataOffset,
				mesh->baseVertex);
			mat->unbind();
		}
	}
	

	//GLFramebuffer::Unbind();

	
	// ImGui::Begin("Framebuffer");
	// {
	// 	ImGui::Image((void*)(framebufferColorTexture.getTextureId()),ImVec2(256,256));                
	// 	ImGui::Image((void*)(renderToFB.getTexture("depthStencil")),ImVec2(128,128));                
	// }
	// ImGui::End();
	//Debug show textures
	//Library::TextureLib->Debug_ShowAllTextures();
	GLVertexArrayObject::Unbind();
}

void ProgramRenderer::shutdown(){

}