#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer(int width, int height, 
    string ssaoProgramName, string ssaoBlurProgramName,
    string shadowMapsProgramName, string stencilProgramName, 
    string pointLightProgramName, string dirLightProgramName):
_mainCamera(nullptr),
_width(width), _height(height),
_deferredWidth(width), _deferredHeight(height),
_gBuffer(width, height),
_ssaoBuffers(width, height, 64),
_shadowMaps(width*2, height*2), 
_debugShadows(false), _debugSSAO(false)
{
    //GBuffer Init
    GLTextureConfiguration locationCFG(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    GLTextureConfiguration colorCFG(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    GLTextureConfiguration depthCFG(width, height, GL_DEPTH32F_STENCIL8,  
        GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    GLTextureConfiguration outputCFG(width, height, GL_RGBA16F, GL_RGB, GL_FLOAT);

    Library::TextureLib->createTexture(POSITION_TEXTURE, locationCFG);
    Library::TextureLib->createTexture(NORMAL_TEXTURE, locationCFG);
    Library::TextureLib->createTexture(COLOR_TEXTURE, colorCFG);
    Library::TextureLib->createTexture(DEPTH_STENCIL_TEXTURE, depthCFG);
    Library::TextureLib->createTexture(COMPOSITE_TEXTURE, outputCFG);
    
    vector<TexParameter> texParams;
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    _gBuffer.addTexture(POSITION_TEXTURE, POSITION_ATTACHMENT, texParams);
    _gBuffer.addTexture(COLOR_TEXTURE, COLOR_ATTACHMENT);
    _gBuffer.addTexture(NORMAL_TEXTURE, NORMAL_ATTACHMENT);
    _gBuffer.addTexture(DEPTH_STENCIL_TEXTURE, GL_DEPTH_ATTACHMENT);
    _gBuffer.addTexture(DEPTH_STENCIL_TEXTURE, GL_STENCIL_ATTACHMENT);
    _gBuffer.addTexture(COMPOSITE_TEXTURE, COMPOSITE_ATTACHMENT);
    _gBuffer.status();

    //ShadowMaps Init
    GLTextureConfiguration shadowCFG(width*2, height*2, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
    texParams.clear();
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    for (auto i = 0; i < NUM_SHADOWS; i++) {
        Library::TextureLib->createTexture(SHADOW_TEXTURE + std::to_string(i), shadowCFG);
        _shadowMaps.addTexture(SHADOW_TEXTURE + std::to_string(i), GL_DEPTH_ATTACHMENT, texParams);
    }
    _shadowMaps.status();

    _renderQuad = MeshCreator::CreateQuad(glm::vec2(-1, -1), glm::vec2(1, 1));

    _shadowMapsProgram = Library::ProgramLib->getProgramForName(shadowMapsProgramName);
    _ssaoProgram = Library::ProgramLib->getProgramForName(ssaoProgramName);
    _ssaoBlurProgram = Library::ProgramLib->getProgramForName(ssaoBlurProgramName);
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
    for(auto step : postprocessPipeline)
    {
        step->setup(window,scene);
    }
}


void DeferredRenderer::render(Scene * scene)
{
    glEnable(GL_CULL_FACE);
    _gBuffer.startFrame();

    _mainCameraPosition = scene->getMainCamera()->getTransform().getPosition();
    glViewport(0,0,_width,_height);

    shadowMapPass(scene);
    geometryPass(scene);

    ImGui::Begin("SSAO Debug");
    {
        ImGui::Checkbox("Disable SSAO", &_debugSSAO);
    }
    ImGui::End();

    if (!_debugSSAO) {
        ssaoPass(scene);
        ssaoBlurPass(scene);
    }

    glEnable(GL_STENCIL_TEST);
    for (auto light : scene->getPointLightObjects()) {
        stencilPass(light);
        pointLightPass(light);
    }
    glDisable(GL_STENCIL_TEST);
    dirLightPass(scene);
    glDisable(GL_CULL_FACE);
    forwardPass(scene);
    glViewport(0,0,_deferredWidth,_deferredHeight);

    _ssaoBuffers.DBG_DrawToImgui("SSAO");
    _gBuffer.DBG_DrawToImgui("GBuffer");
    if(postprocessPipeline.size() == 0)
    {
        outputPass(scene);
    }
    else
    {
        for(auto step : postprocessPipeline)
        {
            step->render(scene);
        }
    }    
    
    
    //outputPass(scene);
    GLVertexArrayObject::Unbind();
}

void DeferredRenderer::shadowMapPass(Scene * scene)
{
    Mesh* mesh;
    GLProgram * activeProgram;
    activeProgram = _shadowMapsProgram;
    activeProgram->enable();
    _shadowMaps.calculateShadowLevels(scene);
    LOG_GL(__FILE__, __LINE__);
    glViewport(0,0,_shadowMaps.getWidth(),_shadowMaps.getHeight());
    glCullFace(GL_FRONT);

    // The view is set as the light source
    glm::mat4 V = _shadowMaps.getLightView();

    for (auto i = 0; i < NUM_SHADOWS; i++) {
        LOG_GL(__FILE__, __LINE__);
        // Bind and clear the current shadowLevel
        _shadowMaps.bindForWriting(i);
        glDrawBuffer(GL_NONE);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 P = _shadowMaps.getOrtho(i);
        

        for (auto obj : scene->getRenderableGameObjectsInFrustrum(P*V))
        {
            if(obj->getTag()!=T_Terrain)
            {
                auto tMat = obj->getComponent<Material>();
                if(tMat->isForward())
                {
                    continue;
                }
                mesh = obj->getComponent<Mesh>();
                mesh->bind();
                glm::mat4 M = obj->getTransform().getMatrix();

                //Place Uniforms that do not change per GameObject
                glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
                glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

                
                glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
                mesh->drawShadow();
            }
        }

    }
    _shadowMaps.DBG_DrawToImgui();
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,_width,_height);
}

void DeferredRenderer::geometryPass(Scene * scene)
{
	GLProgram* activeProgram = nullptr;
	Mesh* mesh;
	Material* mat;

    ImGui::Begin("Shadow Debug");
    {
        ImGui::Checkbox("CSM Levels ", &_debugShadows);
    }
    ImGui::End();

    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();

    
	// Only the geometry pass writes to the depth buffer
    _gBuffer.bindForGeomPass();
	GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


    _shadowMaps.bindForReading();

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    

    for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjectsInFrustrum(P*V))
    {
        mat = obj->getComponent<Material>();
        mesh = obj->getComponent<Mesh>();
        

        if (mat->isForward()) {
            continue;
        }

        glm::mat4 M = obj->getTransform().getMatrix();

		//sets the materials geometry shader as active

        if (activeProgram != mat->getProgram()) {
            activeProgram = mat->getProgram();
            activeProgram->enable();
            setupShadowMapUniforms(activeProgram);
    			//Place Uniforms that do not change per GameObject

            glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
            glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
            if (activeProgram->hasUniform("iGlobalTime")) {
                glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
            }

        }
        mat->bind();
        mesh->bind();
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

void MoonEngine::DeferredRenderer::ssaoPass(Scene * scene)
{
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    _ssaoProgram->enable();
    
    _gBuffer.bindForLightPass();
    _ssaoBuffers.bindForSSAO();
    setupSSAOUniforms(_ssaoProgram);
    glUniformMatrix4fv(_ssaoProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
    glDepthMask(GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
   
    _renderQuad->bind();
    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        _renderQuad->numTris,
        GL_UNSIGNED_SHORT,
        _renderQuad->indexDataOffset,
        _renderQuad->baseVertex
    );

}

void MoonEngine::DeferredRenderer::ssaoBlurPass(Scene * scene)
{
   _ssaoBlurProgram->enable();
   _ssaoBuffers.bindForBlur();
   setupSSAOBlurUniforms(_ssaoBlurProgram);
   glClear(GL_COLOR_BUFFER_BIT);

   _renderQuad->bind();
   glDrawElementsBaseVertex(
       GL_TRIANGLES,
       _renderQuad->numTris,
       GL_UNSIGNED_SHORT,
       _renderQuad->indexDataOffset,
       _renderQuad->baseVertex
   );
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

    
    
    const BasicMeshInfo* lightSphere = light->getComponent<PointLight>()->getSphere();;
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


	
    const BasicMeshInfo* lightSphere = nullptr;
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

    auto dirLight = scene->getDirLightObject()->getComponent<DirLight>();
    glm::vec3 viewLight = glm::vec3(V * glm::vec4(dirLight->getDirection(),0));

    //For every directional light, pass new direction and color
    glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.color"), 1, 
        glm::value_ptr(dirLight->getColor()));
    glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.direction"), 1,
        glm::value_ptr(viewLight));
    glUniform1f(_dirLightProgram->getUniformLocation("dirLight.ambient"), dirLight->getAmbient());

    
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
    _gBuffer.bindForOutput();
    glBlitFramebuffer(0, 0, _width, _height,
        0, 0, _deferredWidth, _deferredHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}


void DeferredRenderer::forwardPass(Scene* scene) {

    _gBuffer.bind(GL_FRAMEBUFFER);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    GLProgram* activeProgram = nullptr;

    Mesh* mesh = nullptr;
    const BasicMeshInfo* meshInfo = nullptr;
    Material* mat = nullptr;
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    LOG_GL(__FILE__, __LINE__);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glEnable(GL_BLEND);

    std::shared_ptr<GameObject> dirLight = scene->getDirLightObject();

    for (std::shared_ptr<GameObject> obj : scene->getForwardGameObjects())
    {

        mat = obj->getComponent<Material>();
        mesh = obj->getComponent<Mesh>();
        mesh->bind();

        glm::mat4 M = obj->getTransform().getMatrix();

        if (activeProgram != mat->getProgram()) {
            activeProgram = mat->getProgram();
            activeProgram->enable();

            //Place Uniforms that do not change per GameObject
            glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
            glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

            if (activeProgram->hasUniform("dirLight.direction"))
            {
                glm::vec3 lDir = dirLight->getComponent<DirLight>()->getDirection();
                glUniform3fv(activeProgram->getUniformLocation("dirLight"), 1,
                     glm::value_ptr(lDir));
            }
        }
        mat->bind();
        
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

void DeferredRenderer::setupShadowMapUniforms(GLProgram * prog)
{
    for (auto i = 0; i < NUM_SHADOWS; i++) {
        auto LV = _shadowMaps.getOrtho(i) * _shadowMaps.getLightView();
        string LVname = "LV[" + to_string(i) + "]";
        glUniformMatrix4fv(prog->getUniformLocation(LVname), 1, GL_FALSE, glm::value_ptr(LV));
    }
    for (auto i = 0; i < NUM_SHADOWS; i++) {
        auto shadowMapName = "shadowMap[" + to_string(i) + "]";
        glUniform1i(prog->getUniformLocation(shadowMapName), 5+i);
    }
    for (auto i = 0; i < NUM_SHADOWS; i++) {
        auto shadowZName = "shadowZSpace[" + to_string(i) + "]";
        glm::vec4 shadowVec(0.0f, 0.0f, _shadowMaps.getShadowZ(i), 1.0f);
        glm::vec4 camShadow = _mainCamera->getProjection() * shadowVec;
        glUniform1f(prog->getUniformLocation(shadowZName), camShadow.z);
    }
    glUniform1i(prog->getUniformLocation("debugShadow"), _debugShadows);
    
}

void DeferredRenderer::setupSSAOUniforms(GLProgram* prog)
{
    int i = 0;
    _gBuffer.UniformTexture(prog, "positionTex", POSITION_TEXTURE);
    _gBuffer.UniformTexture(prog, "normalTex", NORMAL_TEXTURE);
    LOG_GL(__FILE__, __LINE__);
    //Related to the GL_TEXTURExX number in this case 10
    glUniform1i(prog->getUniformLocation("noiseTex"), 10);
    glUniform2f(prog->getUniformLocation("screenSize"), static_cast<float>(_width), static_cast<float>(_height));
    for (auto sample : _ssaoBuffers.getKernel())
        glUniform3fv(prog->getUniformLocation(("kernel[" + to_string(i++) + "]")), 1, &sample[0]);
    
}

void DeferredRenderer::setupSSAOBlurUniforms(GLProgram* prog)
{
    _ssaoBuffers.UniformTexture(prog, "ssaoColor", SSAO_COLOR_TEXTURE);
    glUniform2f(prog->getUniformLocation("screenSize"), static_cast<float>(_width), static_cast<float>(_height));
}

void DeferredRenderer::setupPointLightUniforms(GLProgram * prog, std::shared_ptr<GameObject> light)
{
    auto V = _mainCamera->getView();
    auto P = _mainCamera->getProjection();
    auto M = light->getComponent<PointLight>()->getLightTransform().getMatrix();
    glm::vec3 lightPosition = light->getComponent<PointLight>()->getPosition();
    glm::vec3 viewLightPosition = glm::vec3(V* M * glm::vec4(0,0,0, 1.0));

    //Matrix Uniforms
    glUniformMatrix4fv(prog->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(prog->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(_pointLightProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

    //Texture Uniforms
    _gBuffer.UniformTexture(prog, "positionTex", POSITION_TEXTURE);
    _gBuffer.UniformTexture(prog, "colorTex", COLOR_TEXTURE);
    _gBuffer.UniformTexture(prog, "normalTex", NORMAL_TEXTURE);
    _ssaoBuffers.UniformTexture(prog, "ssaoTex", SSAO_BLUR_TEXTURE);

    //Other global Uniforms
    glUniform2f(prog->getUniformLocation("screenSize"), static_cast<float>(_width),static_cast<float>(_height));

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
    _gBuffer.UniformTexture(prog, "positionTex", POSITION_TEXTURE);
    _gBuffer.UniformTexture(prog, "colorTex", COLOR_TEXTURE);
    _gBuffer.UniformTexture(prog, "normalTex", NORMAL_TEXTURE);
    _ssaoBuffers.UniformTexture(prog, "ssaoTex", SSAO_COLOR_TEXTURE);
    //Other global Uniforms

}


void DeferredRenderer::addPostProcessStep(std::shared_ptr<PostProcessStep> step)
{
    postprocessPipeline.push_back(step);
}

void DeferredRenderer::shutdown()
{

}
