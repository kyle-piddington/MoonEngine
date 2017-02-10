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
    mainCamera(nullptr)
//renderToFB(800,600),
//framebufferColorTexture(0),
//framebufferDepthStencilTexture(1)
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
    if (mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
    //Swing through all rendering components and load their programs.

    glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void ProgramRenderer::render(Scene * scene)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = mainCamera->getView();
    glm::mat4 P = mainCamera->getProjection();

    //No binning

    GLProgram * activeProgram = nullptr;
	int all = scene->getRenderableGameObjects().size();
	int drawn;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjectsInFrustrum(P*V))
    {
        glm::mat4 M = obj->getTransform().getMatrix();
        glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V * M)));
        Material * mat = obj->getComponent<Material>();
		drawn++;
        if (activeProgram != mat->getProgram())
        {
            activeProgram = mat->getProgram();
            activeProgram->enable();

            /* Bind uniforms which change per shader */
            glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));

            glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

            if (activeProgram->hasUniform("iGlobalLightDir"))
            {
                glm::vec3 lightDir = scene->getGlobalLightDir();
                glUniform3f(activeProgram->getUniformLocation("iGlobalLightDir"), lightDir.x, lightDir.y, lightDir.z);
            }
        }
		
        Mesh * meshComp = obj->getComponent<Mesh>();
        const MeshInfo * mesh = obj->getComponent<Mesh>()->getMesh();
        mesh->bind();
        mat->bind();

        /* Bind uniforms which change per object */
        glm::vec3 tint = mat->getTint();
        glUniform3f(activeProgram->getUniformLocation("tint"), tint.x, tint.y, tint.z);

        glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());

        glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix3fv(activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
        meshComp->draw();
        mat->unbind();
    }
	std::cout << "All: " << all << " Rendered: " << drawn << std::endl;
	drawn = 0;
    GLVertexArrayObject::Unbind();

}

void ProgramRenderer::shutdown()
{
}