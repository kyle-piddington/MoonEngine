#include "catch.hpp"
#include "MoonEngine.h"

using namespace MoonEngine;
GLProgram makeProgram()
{
	std::string vertShaderSource = 
	"#version 330 core\n\
	layout (location = 0) in vec3 position;\n\
	uniform vec3 offset;\n\
	void main()\n\
	{\n\
    	gl_Position = vec4(position.x + offset.x, position.y +offset.y, position.z + offset.z, 1.0);\n\
	}\n";
	GLShader vShader(GL_VERTEX_SHADER,vertShaderSource.c_str());
	REQUIRE(vShader.isCompiled());

	std::string fragShaderSource = 
	"#version 330 core\n\
	out vec4 color;\n\
	void main()\n\
	{\n\
    	color = vec4(0,0,0, 1.0);\n\
	}\n";
	GLShader fShader(GL_FRAGMENT_SHADER,fragShaderSource.c_str());
	REQUIRE(fShader.isCompiled());

	GLProgram program = GLProgram("Test program");
	program.attachShader(vShader);
	program.attachShader(fShader);
	program.link();
	return program;
}

GLProgram makeBadProgram()
{
	std::string vertShaderSource = 
	"#version 330 core\n\
	layout (location = 0) in vec3 position;\n\
	void main()\n\
	{\n\
    	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
	}\n";
	GLShader vShader(GL_VERTEX_SHADER,vertShaderSource.c_str());
	REQUIRE(vShader.isCompiled());

	

	GLProgram program = GLProgram("Test program");
	program.attachShader(vShader);
	program.link();
	return program;
}
TEST_CASE("Link basic program", "[Program]")
{
	GLProgram program = makeProgram();
	REQUIRE(program.isReady());
}

TEST_CASE("Link Bad program", "[Program]")
{
	GLProgram program = makeBadProgram();
	REQUIRE(program.isReady());
}

TEST_CASE("Test Uniform Location")
{
	GLProgram prog = makeProgram();
	REQUIRE(prog.getUniformLocation("offset") >= 0);
	REQUIRE(prog.getUniformLocation("offset") >= 0);

}
TEST_CASE("Test Attribute Location")
{
	GLProgram prog = makeProgram();
	REQUIRE(prog.getAttributeLocation("position") >= 0);
	REQUIRE(prog.getAttributeLocation("position") >= 0);
	
}