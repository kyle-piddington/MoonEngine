#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer(int width, int height, string pointLightProgramName, string dirLightProgramName):
    _mainCamera(nullptr),
    _gBuffer(width, height),
    _width(width),
    _height(height),
    _positionTex(),
    _colorTex(),
    _normalTex(),
    _textureTex(),
    _depthTex()
{
    GLTextureConfiguration locationCFG(width, height, GL_RGB16F, GL_RGB, GL_FLOAT);
    GLTextureConfiguration colorCFG(width, height, GL_RGBA, GL_RGBA, GL_FLOAT);
    GLTextureConfiguration depthCFG(width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
    assert(_positionTex.init(locationCFG));
    assert(_colorTex.init(colorCFG));
    assert(_normalTex.init(locationCFG));
    assert(_depthTex.init(depthCFG));
    _gBuffer.addTexture("position", _positionTex, GL_COLOR_ATTACHMENT0);
    
    _gBuffer.addTexture("color", _colorTex, GL_COLOR_ATTACHMENT1);
    _gBuffer.addTexture("normal", _normalTex, GL_COLOR_ATTACHMENT2);
    _gBuffer.addTexture("depth", _depthTex, GL_DEPTH_ATTACHMENT);
    _gBuffer.drawColorAttachments(3);
    _gBuffer.addDepthBuffer();
    LOG_GL(__FILE__, __LINE__);
    _renderQuad = MeshCreator::CreateQuad(glm::vec2(-1, -1), glm::vec2(1, 1));
    _pointLightProgram = Library::ProgramLib->getProgramForName(pointLightProgramName);
    _dirLightProgram = Library::ProgramLib->getProgramForName(dirLightProgramName);
}

void DeferredRenderer::setup(Scene * scene)
{
    _mainCamera = scene->getMainCamera()->getComponent<Camera>();
    _mainCameraPosition = scene->getMainCamera()->getTransform().getPosition();
    if (_mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
    //Swing through all rendering components and load their programs.
    glClearColor(0, 0, 0, 1.0f);
}


void DeferredRenderer::render(Scene * scene)
{
	vector<std::shared_ptr<GameObject>>forwardObjects;
	forwardObjects = geometryPass(scene);
    lightingSetup();
	pointLightPass(scene);
    dirLightPass(scene);
    forwardPass(scene, forwardObjects);
    GLVertexArrayObject::Unbind();
}

vector<std::shared_ptr<GameObject>> DeferredRenderer::geometryPass(Scene * scene)
{
	GLProgram* activeProgram = nullptr;
	const MeshInfo* mesh = nullptr;
	Material* mat = nullptr;
	vector<std::shared_ptr<GameObject>> forwardObjects;
	_gBuffer.bind(GL_DRAW_FRAMEBUFFER);
	glm::mat4 V = _mainCamera->getView();
	glm::mat4 P = _mainCamera->getProjection();

	// Only the geometry pass writes to the depth buffer
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

	for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjects())
	{
		mat = obj->getComponent<Material>();
		mesh = obj->getComponent<Mesh>()->getMesh();
        
		if (mat->isForward()) {
			forwardObjects.push_back(obj);
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
		
        LOG_GL(__FILE__, __LINE__);
		if (obj->getComponent<InstanceMesh>() != nullptr)
		{
			glDrawElementsInstanced(
				GL_TRIANGLES,
				mesh->numTris,
				GL_UNSIGNED_SHORT,
				mesh->indexDataOffset,
				obj->getComponent<InstanceMesh>()->_numOfInstances
			);
		}
		else
		{
			glDrawElementsBaseVertex(
				GL_TRIANGLES,
				mesh->numTris,
				GL_UNSIGNED_SHORT,
				mesh->indexDataOffset,
				mesh->baseVertex
			);
		}
		mat->unbind();
        LOG_GL(__FILE__, __LINE__);
	}

    //Disable the filled depth buffer
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

	//sets the mesh (VAO) back to 0
	return forwardObjects;
}

void MoonEngine::DeferredRenderer::lightingSetup()
{
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    _gBuffer.bindForOutput();
    glClear(GL_COLOR_BUFFER_BIT);
    LOG_GL(__FILE__, __LINE__);
}

void DeferredRenderer::pointLightPass(Scene* scene)
{
    drawBufferToImgui("GBuffer", &_gBuffer);

	const MeshInfo* lightSphere = nullptr;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	_pointLightProgram->enable();
    setupLightUniforms(_pointLightProgram);
    LOG_GL(__FILE__, __LINE__);
	for (std::shared_ptr<GameObject> obj : scene->getPointLightObjects())
	{
		lightSphere = obj->getComponent<PointLight>()->getSphere();
        Transform & t = obj->getComponent<PointLight>()->getLightTransform();
      
        glm::mat4 M = t.getMatrix();

		//sets the point light shader as active
		lightSphere->bind();

        LOG_GL(__FILE__, __LINE__);
        //These values update for every light
		glUniformMatrix4fv(_pointLightProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniform3fv(_pointLightProgram->getUniformLocation("pointLight.color"), 1, 
            glm::value_ptr(obj->getComponent<PointLight>()->getColor()));
        glUniform3fv(_pointLightProgram->getUniformLocation("pointLight.position"), 1,
            glm::value_ptr(obj->getComponent<PointLight>()->getPosition()));

		glUniform1f(_pointLightProgram->getUniformLocation("pointLight.ambient"), obj->getComponent<PointLight>()->getAmbient());

		glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.constant"), obj->getComponent<PointLight>()->getAttenuation().x);
        glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.linear"), obj->getComponent<PointLight>()->getAttenuation().y);
        glUniform1f(_pointLightProgram->getUniformLocation("pointLight.atten.exp"), obj->getComponent<PointLight>()->getAttenuation().z);

        LOG_GL(__FILE__, __LINE__);
		glDrawElementsBaseVertex(
			GL_TRIANGLES,
			lightSphere->numTris,
			GL_UNSIGNED_SHORT,
            lightSphere->indexDataOffset,
            lightSphere->baseVertex
		);

	}
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DeferredRenderer::dirLightPass(Scene* scene)
{
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    _dirLightProgram->enable();
    setupLightUniforms(_dirLightProgram);

    _renderQuad->bind();
    
    
    for (std::shared_ptr<GameObject> obj : scene->getDirLightObjects()) {
        
        DirLight* light = obj->getComponent<DirLight>();
        //For every directional light, pass new direction and color
        glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.color"), 1, 
            glm::value_ptr(obj->getComponent<DirLight>()->getColor()));
        glUniform3fv(_dirLightProgram->getUniformLocation("dirLight.direction"), 1,
            glm::value_ptr(obj->getComponent<DirLight>()->getDirection()));
        glUniform1f(_dirLightProgram->getUniformLocation("dirLight.ambient"), obj->getComponent<DirLight>()->getAmbient());

        
        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            _renderQuad->numTris,
            GL_UNSIGNED_SHORT,
            _renderQuad->indexDataOffset,
            _renderQuad->baseVertex
        );
    }

}


void DeferredRenderer::forwardPass(Scene* scene, vector<std::shared_ptr<GameObject>> forwardObjects) {

    GLProgram* activeProgram = nullptr;
    const MeshInfo* mesh = nullptr;
    Material* mat = nullptr;
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();
    LOG_GL(__FILE__, __LINE__);
    // Only the geometry pass writes to the depth buffer
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    LOG_GL(__FILE__, __LINE__);
    _gBuffer.bind(GL_READ_FRAMEBUFFER);
    LOG_GL(__FILE__, __LINE__);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
    LOG_GL(__FILE__, __LINE__);
    glBlitFramebuffer( 0, 0, _width, _height, 0, 0, _width, _height, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
    LOG_GL(__FILE__, __LINE__);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LOG_GL(__FILE__, __LINE__);
    for (std::shared_ptr<GameObject> obj : forwardObjects)
    {
        mat = obj->getComponent<Material>();
        mesh = obj->getComponent<Mesh>()->getMesh();

        glm::mat4 M = obj->getTransform().getMatrix();

        if (activeProgram != mat->getProgram()) {
            activeProgram = mat->getProgram();
            activeProgram->enable();

            //Place Uniforms that do not change per GameObject
            glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
            glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

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
        if (activeProgram->hasUniform("iGlobalTime")) {
            glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
        }

        if (obj->getComponent<InstanceMesh>() != nullptr)
        {
            glDrawElementsInstanced(
                GL_TRIANGLES,
                mesh->numTris,
                GL_UNSIGNED_SHORT,
                mesh->indexDataOffset,
                obj->getComponent<InstanceMesh>()->_numOfInstances
            );
        }
        else
        {
            glDrawElementsBaseVertex(
                GL_TRIANGLES,
                mesh->numTris,
                GL_UNSIGNED_SHORT,
                mesh->indexDataOffset,
                mesh->baseVertex
            );
        }
        mat->unbind();
    }
    LOG_GL(__FILE__, __LINE__);
    //Disable the filled depth buffer
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

}

void DeferredRenderer::setupLightUniforms(GLProgram * prog)
{
    glm::mat4 V = _mainCamera->getView();
    glm::mat4 P = _mainCamera->getProjection();

    //Place Uniforms that do not change per dirLight
    glUniformMatrix4fv(prog->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(prog->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));

    //Texture Uniforms
    GLFramebuffer::texture_unit id = _gBuffer.getTexture("position");
    glUniform1i(prog->getUniformLocation("positionTex"), id.unit);
    id = _gBuffer.getTexture("color");
    glUniform1i(prog->getUniformLocation("colorTex"), id.unit);
    id = _gBuffer.getTexture("normal");
    glUniform1i(prog->getUniformLocation("normalTex"), id.unit);

    //Other global Uniforms
    glUniform3fv(prog->getUniformLocation("cameraPosition"), 1, glm::value_ptr(_mainCameraPosition));
    glUniform2f(prog->getUniformLocation("screenSize"), (float) _width,(float) _height);
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
