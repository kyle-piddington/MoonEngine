#include "GLProgram.h"
#include "GLUtil/GLProgramUtilities.h"
#include "Util/Logger.h"
using namespace MoonEngine;

GLProgram::GLProgram(std::string name):	
_progName(name)
{
	_progId = glCreateProgram();
}

GLProgram::~GLProgram()
{
	reset();
}

GLProgram::GLProgram(GLProgram && other):
	_progId(other.release()),
	_progName(other._progName),
	_linked(other._linked),
	_attribMap(other._attribMap),
	_uniformMap(other._uniformMap)
{

}

GLProgram& GLProgram::operator=(GLProgram && other)

{
	_progName = other._progName;
	reset(other.release());
	_linked = other._linked;
	_attribMap = other._attribMap;
	_uniformMap = other._uniformMap;

	return *this;	
}


void GLProgram::attachShader(const GLShader & shader)
{
	glAttachShader(_progId, shader.getObject());
}

bool GLProgram::link()
{
	glLinkProgram(_progId);
	_linked = GLProgramUtilities::checkProgramStatus(*this);
	return _linked;
}

bool GLProgram::isReady() const
{
	return _linked;
}



bool GLProgram::enable() const
{
	if(!_linked)
	{
		return false;
	}
	glUseProgram(_progId);
	return true;
}

std::string GLProgram::getName() const
{
	return _progName;
}

GLuint GLProgram::getObject() const
{
	return _progId;
}

GLint GLProgram::getUniformLocation(std::string uniformName)
{
	if(_uniformMap.find(uniformName) == _uniformMap.end())
	{
		GLint unifID = glGetUniformLocation(_progId, uniformName.c_str());
		if(unifID < 0)
		{
			LOG(ERROR, "Uniform " + uniformName + " could not be found in program " + _progName + 
					   "(This error will not be repeated) ");

		}
		_uniformMap[uniformName] = unifID;
	}
	return _uniformMap[uniformName];
}

bool GLProgram::hasUniform(std::string uniformName)
{
	if(_uniformMap.find(uniformName) == _uniformMap.end())
	{
		GLint unifID = glGetUniformLocation(_progId, uniformName.c_str());
		_uniformMap[uniformName] = unifID;
	}
	return _uniformMap[uniformName] != -1;
}

GLint GLProgram::getAttributeLocation(std::string attributeName)
{
	if(_attribMap.find(attributeName) == _attribMap.end())
	{
		GLint unifID = glGetAttribLocation(_progId, attributeName.c_str());
		if(unifID < 0)
		{
			LOG(ERROR, "Attribute " + attributeName + " could not be found in program " + _progName + 
					   "(This error will not be repeated) ");

		}
		_attribMap[attributeName] = unifID;
	}
	return _attribMap[attributeName];
}


GLuint GLProgram::release()
{
	GLuint prog = _progId;
	_progId = 0;
	return prog;
}

GLuint GLProgram::reset(GLuint otherID)
{
	glDeleteProgram(_progId);
	_progId = otherID;
	return _progId;
}


