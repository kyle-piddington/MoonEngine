#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"


using namespace MoonEngine;
GLProgram createProgram()
{
	GLProgram prog("TestProgram");
	GLShader vs  = GLProgramUtilities::createShaderFromFile(GL_VERTEX_SHADER, "resources/flat.vs");
	GLShader fs  = GLProgramUtilities::createShaderFromFile(GL_FRAGMENT_SHADER, "resources/flat.fs");
	prog.attachShader(vs);
	prog.attachShader(fs);
	prog.link();
	return prog;
}	

int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(1920,1080, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// global setup...
	GLProgram program = createProgram();
	float data[] = 
	{
		-0.5,-0.5,0,
		0.5,-0.5,0,
		0,0.5,0
	};

	GLBuffer buffer(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	GLVertexArrayObject obj;
	obj.bindVertexBuffer(0,buffer);
	glClearColor(0.2,0.2,0.6,1.0);
	program.enable();
	while(!glfwWindowShouldClose(window))
	{
	    glfwPollEvents();
	    
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	    obj.bind();
	    glDrawArrays(GL_TRIANGLES,0,3);
	    GLVertexArrayObject::Unbind();
	    glfwSwapBuffers(window);
	}

	
	return 0;

}