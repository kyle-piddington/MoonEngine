#include "DeferredRenderer.h"


using namespace MoonEngine;


DeferredRenderer::DeferredRenderer(int width, int height):
_mainCamera(nullptr),
_gBuffer(width, height),
_width(width),
_height(height),
_positionTex(0),
_colorTex(1),
_normalTex(2),
_textureTex(3),
_depthTex(4)
{
    // renderQuad = MeshCreator::CreateQuad(glm::vec2(-1,1), glm::vec2(1,1));
	GLTextureConfiguration colorCFG(width, height, GL_RGB32F, GL_RGB, GL_FLOAT);
	GLTextureConfiguration depthCFG(width, height, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
	assert(_positionTex.init(colorCFG));
	assert(_colorTex.init(colorCFG));
	assert(_normalTex.init(colorCFG));
	assert(_textureTex.init(colorCFG));
	assert(_depthTex.init(depthCFG));
	
	_positionTex.init(colorCFG);
	
	_gBuffer.addTexture("position",_positionTex,GL_COLOR_ATTACHMENT0);
	_gBuffer.addTexture("color", _colorTex, GL_COLOR_ATTACHMENT1);
	_gBuffer.addTexture("normal", _normalTex, GL_COLOR_ATTACHMENT2);
	_gBuffer.addTexture("texture", _textureTex, GL_COLOR_ATTACHMENT3);
    _gBuffer.addTexture("depth",_depthTex,GL_DEPTH_ATTACHMENT);

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
    glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}


void DeferredRenderer::render(Scene * scene)
{

	vector<std::shared_ptr<GameObject>>forwardObjects;
        
	forwardObjects = geometryPass(scene);
	lightingPass(scene);
     ImGui::Begin("Framebuffer");
     {
     	ImGui::Image((void*)(_colorTex.getTextureId()),ImVec2(256,256));
     	ImGui::Image((void*)(_gBuffer.getTexture("depth")),ImVec2(128,128));
     }
     ImGui::End();
    //Debug show textures
    Library::TextureLib->Debug_ShowAllTextures();
    GLVertexArrayObject::Unbind();
}

vector<std::shared_ptr<GameObject>> DeferredRenderer::geometryPass(Scene * scene)
{
	GLProgram* activeProgram = nullptr;
	const MeshInfo* mesh = nullptr;
	Material* mat = nullptr;
	vector<std::shared_ptr<GameObject>> forwardObjects;
	
	_gBuffer.drawColorAttachments();
	_gBuffer.bind(GL_DRAW_FRAMEBUFFER);
	glm::mat4 V = _mainCamera->getView();
	glm::mat4 P = _mainCamera->getProjection();
	
	// Only the geometry pass writes to the depth buffer
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

			//No assumptions about the geometry stage is made beyond a P, V, and M Uniforms
			glUniformMatrix4fv(activeProgram->getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));
			glUniformMatrix4fv(activeProgram->getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
			glUniformMatrix4fv(activeProgram->getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));

			//Optional Uniforms are checked here, and bound if found
			if (activeProgram->hasUniform("tint")) {
				glm::vec3 tint = mat->getTint();
				glUniform3f(activeProgram->getUniformLocation("tint"), tint.x, tint.y, tint.z);
			}
			if (activeProgram->hasUniform("iGLobalTime")) {
				glUniform1f(activeProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
			}
			if (activeProgram->hasUniform("N")) {
				glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V * M)));
				glUniformMatrix3fv(activeProgram->getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));
			}

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
	//sets the framebuffer back to the default(0)
	GLFramebuffer::Unbind();
	//sets the mesh (VAO) back to 0
	GLVertexArrayObject::Unbind();
	return forwardObjects;
}

void DeferredRenderer::lightingPass(Scene * scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_gBuffer.bind(GL_READ_FRAMEBUFFER);

	GLuint halfWidth = (GLuint)_width / 2.0f;
	GLuint halfHeight = (GLuint)_height / 2.0f;

	_gBuffer.setReadBuffer("position");
	glBlitFramebuffer(0, 0, _width, _height, 
		0, 0, halfWidth, halfHeight, 
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_gBuffer.setReadBuffer("color");
	glBlitFramebuffer(0, 0, _width, _height, 
		0, halfHeight, halfWidth, _height, 
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_gBuffer.setReadBuffer("normal");
	glBlitFramebuffer(0, 0, _width, _height, 
		halfWidth, halfHeight, _width, _height, 
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_gBuffer.setReadBuffer("texture");
	glBlitFramebuffer(0, 0, _width, _height, 
		halfWidth, 0, _width, halfHeight, 
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

}


void DeferredRenderer::shutdown()
{
}


