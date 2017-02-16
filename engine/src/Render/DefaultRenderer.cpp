#include "DefaultRenderer.h"
#include "Util/Logger.h"
#include "GLWrapper/OpenGL.h"
#include <glm/gtc/type_ptr.hpp>
#include "Component/Components.h"
#include "GameObject/GameObject.h"

using namespace MoonEngine;

GLProgram createBasicPhongProgram()
{
    //Hardcoded code to implement phong shading here.
    std::string vShaderText =
            "#version 330 core\n\
		layout (location = 0) in vec4 position;\n\
		layout (location = 1) in vec3 normal;\n\
		uniform mat4 M;\n\
		uniform mat4 V;\n\
		uniform mat4 P;\n\
		uniform mat3 N;\n\
		out vec3 fragPos;\n\
		out vec3 fragNor;\n\
		void main()\n\
		{\n\
		   gl_Position = P * V * M *  position;\n\
		   fragPos = vec3(V * M * position);\n\
		   fragNor = N * normal;\n\
		}";


    std::string fShaderText =
            "#version 330 core\n\
	in vec3 fragPos;\n\
	in vec3 fragNor;\n\
	out vec4 color;\n\
	uniform vec3 tint;\n\
	uniform vec3 lightDirWorld;\n\
	uniform mat4 V;\n\
	void main()\n\
	{\n\
	   vec3 lightDir = normalize(vec3(V * vec4(lightDirWorld,0.0)));\n\
	   vec3 ambient = 0.3 * tint;\n\
	   vec3 nor=normalize(fragNor);\n\
	   float diff = max(dot(nor,lightDir),0.0f);\n\
	   vec3 diffuse = diff * tint;\n\
	   vec3 reflectDir = reflect(-lightDir, nor);\n\
	   vec3 viewDir = normalize(vec3(0,0,1) - fragPos);\n\
	   float spec = pow(max(dot(reflectDir,viewDir),0.0),32);\n\
	   vec3 specular = spec * vec3(1,1,1);\n\
	   vec3 result = (diffuse + ambient + specular);\n\
	   color = vec4(result,1.0);\n}";
    GLShader vShader = GLShader(GL_VERTEX_SHADER, vShaderText.c_str());
    GLShader fShader = GLShader(GL_FRAGMENT_SHADER, fShaderText.c_str());
    GLProgram phongProg("Default_Phong");
    phongProg.attachShader(vShader);
    phongProg.attachShader(fShader);
    phongProg.link();
    assert(phongProg.isReady());
    return phongProg;
}

DefaultRenderer::DefaultRenderer():
        basicPhongProgram("blank"),
        mainCamera(nullptr)
{

}

void DefaultRenderer::setup(Scene * scene, GLFWwindow * window)
{
    mainCamera = scene->findGameObjectWithComponent<Camera>()->getComponent<Camera>();
    if (mainCamera == nullptr)
    {
        LOG(ERROR, "No Camera in scene!");
    }
    basicPhongProgram = createBasicPhongProgram();
    glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void DefaultRenderer::render(Scene * scene)
{
    //Bind program
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    basicPhongProgram.enable();
    glm::mat4 V = mainCamera->getView();
    glm::mat4 P = mainCamera->getProjection();
    glUniformMatrix4fv(
        basicPhongProgram.getUniformLocation("P"), 1, GL_FALSE, glm::value_ptr(P));

    glUniformMatrix4fv(
        basicPhongProgram.getUniformLocation("V"), 1, GL_FALSE, glm::value_ptr(V));
    //No binning
    //World position light
    glm::vec3 lightDir(1, 1, 1);
    glUniform3f(basicPhongProgram.getUniformLocation("lightDirWorld"), lightDir.x, lightDir.y, lightDir.z);
    for (std::shared_ptr<GameObject> obj : scene->getRenderableGameObjects())
    {
        glm::mat4 M = obj->getTransform().getMatrix();
        glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(V * M)));
        Material * mat = obj->getComponent<Material>();
        glm::vec3 tint = mat->getTint();
        const MeshInfo * mesh = obj->getComponent<StaticMesh>()->getMesh();
        mesh->bind();
        glUniform3f(basicPhongProgram.getUniformLocation("tint"), tint.x, tint.y, tint.z);
        glUniformMatrix4fv(
            basicPhongProgram.getUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix3fv(
            basicPhongProgram.getUniformLocation("N"), 1, GL_FALSE, glm::value_ptr(N));

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            mesh->numTris,
            GL_UNSIGNED_SHORT,
            mesh->indexDataOffset,
            mesh->baseVertex
        );
    }
    GLVertexArrayObject::Unbind();
}

void DefaultRenderer::shutdown()
{
}