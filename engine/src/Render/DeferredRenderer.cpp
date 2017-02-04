#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer():
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

void DeferredRenderer::setup(Scene * scene)
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

void DeferredRenderer::render(Scene * scene)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 V = mainCamera->getView();
    glm::mat4 P = mainCamera->getProjection();
	

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

void DeferredRenderer::shutdown()
{
}