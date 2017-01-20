// #include "DefaultRenderer.h"
// #include "Util/Logger.h"
// #include "GL/OpenGL.h"

// GLProgram createBasicPhongProgram()
// {
// 	//Hardcoded code to implement phong shading here.

// }

// DefaultRenderer::setup(const Scene * scene)
// {
// 	Camera * cam = scene->findGameObjectWithComponent<Camera>().getComponent<Camera>();
// 	if(cam == nullptr)
// 	{
// 		LOG(ERROR , "No Camera in scene!");
// 	}
// 	basicPhongProgram = createBasicPhongProgram();
// }

// DefaultRenderer::render(const Scene * scene)
// {
// 	//Bind program
// 	basicPhongProgram.enable();
// 	glm::mat4 PV = mainCamera->getProjection() * mainCamera->getView();
// 	glUniformMatrix4fv(
// 		basicPhongProgram->getUniformLocation("PV"),1,glm::value_ptr(PV));
// 	//No binning
// 	//World position light
// 	glm::vec3 lightDir(-1,-1,-1);
// 	glUniform3f(basePhongProgram->getUniformLocation("lightDir"),lightDir.x,lightDir.y,lightDir.z);
// 	for(const GameObject * obj : scene->getGameObjects())
// 	{
// 		glm::mat4 M = obj->getTransform().getMatrix();
// 		glm::vec3 tint = obj->getComponent<Material>()->getTint();
// 		StaticMesh * mesh = obj->getComponent<StaticMesh>()->bindVAO();
// 		glUniform3f(basicPhongProgram->getUniformLocation("color"),tint.x,tint.y,tint.z);
// 		glUniformMatrix4fv(
// 			basicPhongProgram->getUniformLocation("M"),1,glm::value_ptr(M));
// 		glDrawElementsBaseVertex(GL_TRIANGLES, 
// 			mesh->numTris, 
// 			GL_UNSIGNED_SHORT, 
// 			mesh->indexDataOffset,
// 			object->baseVertex);
// 	}
// 	GLVertexArrayObject.Unbind();
// }