#include "ProgramRenderer.h"
#include "Util/Logger.h"
#include "GL/OpenGL.h"
#include <glm/gtc/type_ptr.hpp>
#include "Component/Components.h"
#include "GameObject/GameObject.h"
using namespace MoonEngine;



ProgramRenderer::ProgramRenderer():
	mainCamera(nullptr)
{

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
	//Bind program
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 V =  mainCamera->getView();
	glm::mat4 P = mainCamera->getProjection();
	//No binning
	//World position light
	//(Hardcoded for now)
	glm::vec3 lightDir(1,1,1);
	GLProgram * activeProgram = nullptr;
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
		const MeshInfo * mesh = obj->getComponent<StaticMesh>()->getMesh();
		mesh->bind();
		//@TODO: Refactor these later
		glUniform3f(activeProgram->getUniformLocation("tint"),tint.x,tint.y,tint.z);
		glUniformMatrix4fv(
			activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
		glUniformMatrix3fv(
			activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
		
		glDrawElementsBaseVertex(GL_TRIANGLES, 
			mesh->numTris, 
			GL_UNSIGNED_SHORT, 
			mesh->indexDataOffset,
			mesh->baseVertex);
	}
	GLVertexArrayObject::Unbind();
}

void ProgramRenderer::shutdown(){

}