#include "GLProgramUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Logger.h"
using namespace MoonEngine;

GLShader GLProgramUtilities::createShaderFromFile(GLenum shaderType, char * fileName)
{
	std::ifstream shaderSource(fileName);
	if(shaderSource.is_open())
	{
		std::stringstream buffer;
		buffer << shaderSource.rdbuf();
		std::string shaderStr = buffer.str();
		return GLShader(shaderType, shaderStr.c_str());
	}
	else
	{
		LOG(ERROR, "Could not open file named " + std::string(fileName));
		return GLShader();
	}

}

/* Source; https://learnopengl.com/#!Getting-started/Hello-Triangle*/
bool GLProgramUtilities::checkShaderStatus(const GLShader & shader)
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader.getObject(), GL_COMPILE_STATUS, &success);

	if(!success)
	{
	    glGetShaderInfoLog(shader.getObject(), 512, NULL, infoLog);
	    std::string shaderType;
	    switch(shader.getShaderType())
	    {
	    	case  GL_COMPUTE_SHADER:
	    			shaderType = "COMPUTE";
	    			break;
	    	case GL_VERTEX_SHADER:
	    			shaderType = "VERTEX";
	    			break;
	    	case GL_TESS_CONTROL_SHADER:
	    			shaderType = "TESS_CONTROL";
	    			break;
	    	case GL_TESS_EVALUATION_SHADER:
	    			shaderType = "TESS_EVAL";
	    			break;
	    	case  GL_GEOMETRY_SHADER:
	    			shaderType = "GEOMETRY";
	    			break;
	    	case  GL_FRAGMENT_SHADER:
	    			shaderType = "FRAGMENT";
	    			break;
	    	default:
	    		shaderType = "UNKNOWN";
	    }
	    LOG(ERROR,"SHADER:" + shaderType + ":COMPILATION_FAILED\n" + std::string(infoLog));
	}

	return success;
}

bool GLProgramUtilities::checkProgramStatus(const GLProgram & program)
{
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program.getObject(), GL_LINK_STATUS, &success);
	if(!success)
	{
		  	glGetProgramInfoLog(program.getObject(), 512, NULL, infoLog);
		    LOG(ERROR,"PROGRAM:[" + program.getName() + "]:LINK_FAILED\n" + std::string(infoLog));
	}
	return success;
}