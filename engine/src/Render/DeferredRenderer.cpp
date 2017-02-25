#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer(int width, int height, string stencilProgramName, 
    string pointLightProgramName, string dirLightProgramName):
    _mainCamera(nullptr),
    _gBuffer(width, height),
    _width(width),
    _height(height),
    _positionTex(),
    _colorTex(),
    _normalTex(),
    _textureTex(),
    _depthTex(),
    _outputTex()
{
    GLTextureConfiguration locationCFG(width, height, GL_RGB16F, GL_RGB, GL_FLOAT);
    GLTextureConfiguration colorCFG(width, height, GL_RGBA, GL_RGBA, GL_FLOAT);
    GLTextureConfiguration depthCFG(width, height, GL_DEPTH32F_STENCIL8, 
        GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    GLTextureConfiguration outputCFG(width, height, GL_RGBA, GL_RGB, GL_FLOAT);

    
    if (_positionTex.init(locationCFG) == false) 
        exit(EXIT_FAILURE);
    if (_colorTex.init(colorCFG) == false) 
        exit(EXIT_FAILURE);
    if (_normalTex.init(locationCFG) == false)
        exit(EXIT_FAILURE);
    if (_depthTex.init(depthCFG) == false)
        exit(EXIT_FAILURE);
    if (_outputTex.init(outputCFG) == false)
        exit(EXIT_FAILURE);

    _gBuffer.addTexture("position", _positionTex, GL_COLOR_ATTACHMENT0);
    _gBuffer.addTexture("color", _colorTex, GL_COLOR_ATTACHMENT1);
    _gBuffer.addTexture("normal", _normalTex, GL_COLOR_ATTACHMENT2);

    _gBuffer.addTexture("depth", _depthTex, GL_DEPTH_ATTACHMENT);
    _gBuffer.addTexture("stencil", _depthTex, GL_STENCIL_ATTACHMENT);

    _gBuffer.addTexture("output", _outputTex, GL_COLOR_ATTACHMENT4);
    _gBuffer.status();

    _renderQuad = MeshCreator::CreateQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
    _stencilProgram = Library::ProgramLib->getProgramForName(stencilProgramName);
    _pointLightProgram = Library::ProgramLib->getProgramForName(pointLightProgramName);
    _dirLightProgram = Library::ProgramLib->getProgramForName(dirLightProgramName);

    
    
}

void DeferredRenderer::setup(Scene * scene, GLFWwindow * window)
{
    _mainCamera = scene->getMainCamera()->getComponent<Camera>();
    if (_mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
    glfwGetFramebufferSize(window, &_deferredWidth, &_deferredHeight);
    glClearColor(0, 0, 0, 1.0f);
}


void DeferredRenderer::render(Scene * scene)
{
   _gBuffer.startFrame();

   _mainCameraPosition = scene->getMainCamera()->getTransform().getPosition();
   glViewport(0,0,_width,_height);
   geometryPass(scene);

    glEnable(GL_STENCIL_TEST);
    for (std::shared_ptr<GameObject> light : scene->getPointLightObjects()) {
        stencilPass(light);
        pointLightPass(light);
    }
    glDisable(GL_STENCIL_TEST);
    
    dirLightPass(scene);
    glViewport(0,0,_deferredWidth,_deferredHeight);
    outputPass(scene);
    //forwardPass(scene);

    GLVertexArrayObject::Unbind();
}

void DeferredRenderer::geometryPass(Scene * scene)
{
	GLProgram* activeProgram = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;

    glm::mat4 V = _mainCamera->getView();
	glm::mat4 P = _mainCamera->getProjection();

    
	// Only the geometry pass writes to the depth buffer
    _gBuffer.bindForGeomPass();
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
	for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjectsInFrustrum(P*V))
	{
		mat = obj->getComponent<Material>();
		mesh = obj->getComponent<Mesh>();
        const MeshInfo * meshInfo = mesh->getMesh();
        
		if (mat->isForward()) {
			continue;
		}

		glm::mat4 M = obj->getTransform().getMatrix();
		
		//sets the materials geometry shader as active
        
		if (activeProgram != mat->getProgram()) {
			activeProgram = mat->getProgram();
			activeProgram->enable();

			//Place Uniforms that do not change per GameObject
			glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
			glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
			if (activeProgram->hasUniform("iGlobalTime")) {
				glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
			}
            
		}
		mat->bind();
		meshInfo->bind();
		//No assumptions about the geometry stage is made beyond a P, V, and M Uniforms
		glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));


		//Optional Uniforms are checked here, and bound if found
		if (activeProgram->hasUniform("tint")) {
			glm::vec3 tint = mat->getTint();
			glUniform3f(activeProgram->getUniformLocation("tint"), tint.x, tint.y, tint.z);
		}
		if (activeProgram->hasUniform("N")) {
			glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V * M)));
			glUniformMatrix3fv(activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
		}
		
		mesh->draw();
		mat->unbind();
	}

    //Disable the filled depth buffer
    glDepthMask(GL_FALSE);
}

void MoonEngine::DeferredRenderer::stencilPass(std::shared_ptr<GameObject> light)
{
    //setup NULL shaders

    _stencilProgram->enable();
    _gBuffer.bindForStencilPass();
    glEnable(GL_DEPTH_TEST);

    
    glDisable(GL_CULL_FACE);
    glStencilMask(0xFF);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    //enable stencil but set it to always pass
    glStencilFunc(GL_ALWAYS, 0,0xFF);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    
    
    const MeshInfo* lightSphere = light->getComponent<PointLight>()->getSphere();;
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    glm::mat4 M = light->getComponent<PointLight>()->getLightTransform().getMatrix();

    glUniformMatrix4fv(_stencilProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(_stencilProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(_stencilProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

    lightSphere->bind();

    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        lightSphere->numTris,
        GL_UNSIGNED_SHORT,
        lightSphere->indexDataOffset,
        lightSphere->baseVertex
    );
    glStencilMask(0x00);

}

void DeferredRenderer::pointLightPass(std::shared_ptr<GameObject> light)
{
    _pointLightProgram->enable();
    _gBuffer.bindForLightPass();
    setupPointLightUniforms(_pointLightProgram, light);
	
    const MeshInfo* lightSphere = nullptr;
    lightSphere = light->getComponent<PointLight>()->getSphere();

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);


    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
  

	lightSphere->bind();

	glDrawElementsBaseVertex(
		GL_TRIANGLES,
		lightSphere->numTris,
		GL_UNSIGNED_SHORT,
        lightSphere->indexDataOffset,
        lightSphere->baseVertex
	);

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}

void DeferredRenderer::dirLightPass(Scene* scene)
{
    glm::mat4 V = _mainCamera->getView();

    _dirLightProgram->enable();
    setupDirLightUniforms(_dirLightProgram);
    _renderQuad->bind();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    std::shared_ptr<GameObject> dirLight = scene->getDirLightObject();
    glm::vec3 viewLight = 
        glm::vec3(V * glm::vec4(dirLight->getComponent<DirLight>()->getDirection(),0));

    DirLight* light = dirLight->getComponent<DirLight>();
    //For every directional light, pass new direction and color
    glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.color"), 1, 
        glm::value_ptr(dirLight->getComponent<DirLight>()->getColor()));
    glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.direction"), 1,
        glm::value_ptr(viewLight));
    glUniform1f(_dirLightProgram->getUniformLocation("dirLight.ambient"), dirLight->getComponent<DirLight>()->getAmbient());

    
    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        _renderQuad->numTris,
        GL_UNSIGNED_SHORT,
        _renderQuad->indexDataOffset,
        _renderQuad->baseVertex
    );
    glDisable(GL_BLEND);

}

void DeferredRenderer::outputPass(Scene * scene){
    drawBufferToImgui("GBuffer", &_gBuffer);
    _gBuffer.bindForOutput();
    glBlitFramebuffer(0, 0, _width, _height,
        0, 0, _deferredWidth, _deferredHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}


void DeferredRenderer::forwardPass(Scene* scene) {
    
    GLProgram* activeProgram = nullptr;
    Mesh* mesh = nullptr;
    const MeshInfo* meshInfo = nullptr;
    Material* mat = nullptr;
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    LOG_GL(__FILE__, __LINE__);
    // Only the geometry pass writes to the depth buffer
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);


    for (std::shared_ptr<GameObject> obj : scene->getForwardGameObjects())
    {
        mat = obj->getComponent<Material>();
        mesh = obj->getComponent<Mesh>();
        meshInfo = mesh->getMesh();

        glm::mat4 M = obj->getTransform().getMatrix();

        if (activeProgram != mat->getProgram()) {
            activeProgram = mat->getProgram();
            activeProgram->enable();

            //Place Uniforms that do not change per GameObject
            glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
            glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

        }
        mat->bind();
        meshInfo->bind();
        //No assumptions about the geometry stage is made beyond a P, V, and M Uniforms
        glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));


        //Optional Uniforms are checked here, and bound if found
        if (activeProgram->hasUniform("tint")) {
            glm::vec3 tint = mat->getTint();
            glUniform3f(activeProgram->getUniformLocation("tint"), tint.x, tint.y, tint.z);
        }
        if (activeProgram->hasUniform("N")) {
            glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V * M)));
            glUniformMatrix3fv(activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
        }
        if (activeProgram->hasUniform("iGlobalTime")) {
            glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
        }
        mesh->draw();

       
        mat->unbind();
    }
    LOG_GL(__FILE__, __LINE__);
    //Disable the filled depth buffer
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

}

void DeferredRenderer::setupPointLightUniforms(GLProgram * prog, std::shared_ptr<GameObject> light)
{
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    glm::mat4 M = light->getComponent<PointLight>()->getLightTransform().getMatrix();
    glm::vec3 lightPosition = light->getComponent<PointLight>()->getPosition();
    glm::vec3 viewLightPosition = glm::vec3(V* M * glm::vec4(0,0,0, 1.0));

    //Matrix Uniforms
    glUniformMatrix4fv(prog->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(prog->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(_pointLightProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

    //Texture Uniforms
    GLFramebuffer::texture_unit id = _gBuffer.getTexture("position");
    glUniform1i(prog->getUniformLocation("positionTex"), id.unit);
    id = _gBuffer.getTexture("color");
    glUniform1i(prog->getUniformLocation("colorTex"), id.unit);
    id = _gBuffer.getTexture("normal");
    glUniform1i(prog->getUniformLocation("normalTex"), id.unit);

    //Other global Uniforms
    glUniform2f(prog->getUniformLocation("screenSize"), (float) _width,(float) _height);

    //PointLight Specific Uniforms
    glUniform3fv(_pointLightProgram->getUniformLocation("pointLight.color"), 1,
        glm::value_ptr(light->getComponent<PointLight>()->getColor()));
    glUniform3fv(_pointLightProgram->getUniformLocation("pointLight.position"), 1,
        glm::value_ptr(viewLightPosition));

    glUniform1f(_pointLightProgram->getUniformLocation("pointLight.ambient"), light->getComponent<PointLight>()->getAmbient());

    glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.constant"), light->getComponent<PointLight>()->getAttenuation().x);
    glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.linear"), light->getComponent<PointLight>()->getAttenuation().y);
    glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.exp"), light->getComponent<PointLight>()->getAttenuation().z);

}

void MoonEngine::DeferredRenderer::setupDirLightUniforms(GLProgram * prog)
{
    //Texture Uniforms
    GLFramebuffer::texture_unit id = _gBuffer.getTexture("position");
    glUniform1i(prog->getUniformLocation("positionTex"), id.unit);
    id = _gBuffer.getTexture("color");
    glUniform1i(prog->getUniformLocation("colorTex"), id.unit);
    id = _gBuffer.getTexture("normal");
    glUniform1i(prog->getUniformLocation("normalTex"), id.unit);

    //Other global Uniforms

}

void DeferredRenderer::shutdown()
{

}

void MoonEngine::drawBufferToImgui(std::string guiName, const GLFramebuffer* bfr)
{
    auto texHandles = bfr->getTextureHandles();
    ImGui::Begin(guiName.c_str());
    for (auto texHandlePair : texHandles)
    {
        ImGui::Image((void *)texHandlePair.second.gl_texture->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}
