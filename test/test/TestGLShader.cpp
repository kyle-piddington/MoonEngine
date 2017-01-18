#include "catch.hpp"
#include "MoonEngine.h"
using namespace MoonEngine;
TEST_CASE("Vertex Shader Compile", "[Shader]")
{
	std::string shaderSource = 
	"#version 330 core\n\
	layout (location = 0) in vec3 position;\n\
	void main()\n\
	{\n\
    	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
	}\n";
	GLShader shader(GL_VERTEX_SHADER,shaderSource.c_str());
	REQUIRE(shader.isCompiled());
}
TEST_CASE("Vertex Shader Non-Compile", "[Shader]")
{
	GLShader shader(GL_VERTEX_SHADER, "NoShaderComp");
	REQUIRE(shader.isCompiled() == false);
}

GLShader genShader(const char * source, GLuint * capValue)
{

	GLShader bufA =  GLShader(GL_VERTEX_SHADER, source);
	*capValue = bufA.getObject();
	return bufA;
}

TEST_CASE("GLShader sanity tests", "[GLShader]")
{
	//Test getting a shader
	std::string shaderSource = 
	"#version 330 core\n\
	layout (location = 0) in vec3 position;\n\
	void main()\n\
	{\n\
    	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
	}\n";
	GLShader bufferA = GLShader(GL_VERTEX_SHADER, shaderSource.c_str());
	REQUIRE(bufferA.getObject() != 0);
	GLuint bufferObject;
	//Test move constructor
	GLShader bufferB = genShader(shaderSource.c_str(),&bufferObject);
	REQUIRE(bufferB.getObject() == bufferObject);
}