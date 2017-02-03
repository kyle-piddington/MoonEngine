#include "GLShader.h"
#include "GLUtil/GLProgramUtilities.h"

using namespace MoonEngine;


GLShader::GLShader():
    compiled(false)
{
}

GLShader::GLShader(GLenum shaderType, const char * source):
    _shaderType(shaderType)
{
    _objectId = glCreateShader(shaderType);
    glShaderSource(_objectId, 1, &source, NULL);
    glCompileShader(_objectId);
    compiled = GLProgramUtilities::checkShaderStatus(*this);
}

GLShader::~GLShader()
{
    reset();
}

GLShader::GLShader(GLShader && other):
    _objectId(other.release()),
    _shaderType(other._shaderType)
{
}

GLShader & GLShader::operator=(GLShader && other)
{
    reset(other.release());
    _shaderType = other._shaderType;
    return *this;
}

bool GLShader::isCompiled() const
{
    return compiled;
}

GLuint GLShader::getObject() const
{
    return _objectId;
}


GLenum GLShader::getShaderType() const
{
    return _shaderType;
}

GLuint GLShader::release()
{
    GLuint oldId = _objectId;
    _objectId = 0;
    return oldId;
}

GLuint GLShader::reset(GLuint newObject)
{
    glDeleteShader(_objectId);
    _objectId = newObject;
    return _objectId;
}