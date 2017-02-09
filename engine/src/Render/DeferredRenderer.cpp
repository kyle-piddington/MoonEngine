#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer(int width, int height):
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
    // renderQuad = MeshCreator::CreateQuad(glm::vec2(-1,1), glm::vec2(1,1));
    GLTextureConfiguration colorCFG(width, height, GL_RGB32F, GL_RGB, GL_FLOAT);
    GLTextureConfiguration depthCFG(width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
    assert(_positionTex.init(colorCFG));
    assert(_colorTex.init(colorCFG));
    assert(_normalTex.init(colorCFG));
    assert(_textureTex.init(colorCFG));
    assert(_depthTex.init(depthCFG));
    _gBuffer.addTexture("position", _positionTex, GL_COLOR_ATTACHMENT0);
    _gBuffer.addTexture("color", _colorTex, GL_COLOR_ATTACHMENT1);
    _gBuffer.addTexture("normal", _normalTex, GL_COLOR_ATTACHMENT2);
    _gBuffer.addTexture("texture", _textureTex, GL_COLOR_ATTACHMENT3);
    _gBuffer.addTexture("depth", _depthTex, GL_DEPTH_ATTACHMENT);
    _gBuffer.drawColorAttachments(4);
}

void DeferredRenderer::setup(Scene * scene)
{
    _mainCamera = scene->findGameObjectWithComponent<Camera>()->getComponent<Camera>();
    if (_mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
    //Swing through all rendering components and load their programs.
    glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
}


void DeferredRenderer::render(Scene * scene)
{
	vector<std::shared_ptr<GameObject>>forwardObjects;
	forwardObjects = geometryPass(scene);
    lightingSetup();
	pointLightingPass(scene);
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
		mat->setActiveProgram(0);
		mat->bind();
		mesh->bind();
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

    //Disable the filled depth buffer
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

	//sets the framebuffer back to the default(0)
	GLFramebuffer::Unbind();
	//sets the mesh (VAO) back to 0
	GLVertexArrayObject::Unbind();
	return forwardObjects;
}

void MoonEngine::DeferredRenderer::lightingSetup()
{
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    _gBuffer.bind(GL_READ_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DeferredRenderer::pointLightingPass(Scene* scene)
{
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    drawBufferToImgui("GBuffer", &_gBuffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    GLuint halfWidth = (GLuint) _width / 2.0f;
    GLuint halfHeight = (GLuint) _height / 2.0f;


    GLProgram* activeProgram = nullptr;
	const MeshInfo* lightSphere = nullptr;
	Material* mat = nullptr;
	glm::mat4 V = _mainCamera->getView();
	glm::mat4 P = _mainCamera->getProjection();

	for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjects())
	{
		mat = obj->getComponent<Material>();
		lightSphere = obj->getComponent<PointLight>()->getSphere();

		glm::mat4 M = obj->getTransform().getMatrix();
		
		//sets the point light shader as active
		mat->setActiveProgram(1);
		mat->bind();
		lightSphere->bind();
		if (activeProgram != mat->getProgram()) {
			activeProgram = mat->getProgram();
			activeProgram->enable();

			//Place Uniforms that do not change per Light
			glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
			glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
            glUniform1f(activeProgram->getUniformLocation("pointLight.ambient"), obj->getComponent<PointLight>()->getAmbient());
            glUniform1f(activeProgram->getUniformLocation("pointLight.intensity"), obj->getComponent<PointLight>()->getIntensity());
		}

		//Assumptions about the light points are: P, V, M Matrices 
        //and the point light values

        //These values update for every light
		glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniform3fv(activeProgram->getUniformLocation("pointLight.color"), 3, 
            glm::value_ptr(obj->getComponent<PointLight>()->getColor()));
        glUniform3fv(activeProgram->getUniformLocation("pointLight.position"), 3,
            glm::value_ptr(obj->getComponent<PointLight>()->getPosition()));

        glUniform1f(activeProgram->getUniformLocation("atten.constant"), obj->getComponent<PointLight>()->getAttenuation().x);
        glUniform1f(activeProgram->getUniformLocation("atten.linear"), obj->getComponent<PointLight>()->getAttenuation().y);
        glUniform1f(activeProgram->getUniformLocation("atten.exp"), obj->getComponent<PointLight>()->getAttenuation().z);


		glDrawElementsBaseVertex(
			GL_TRIANGLES,
			lightSphere->numTris,
			GL_UNSIGNED_SHORT,
            lightSphere->indexDataOffset,
            lightSphere->baseVertex
		);
		mat->unbind();
	}


}

void DeferredRenderer::directionalLightingPass(Scene* scene)
{

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
        ImGui::Image((void *)texHandlePair.second, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}
