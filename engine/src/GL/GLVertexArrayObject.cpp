#include "GLVertexArrayObject.h"
#include "Util/Logger.h"
#include <cassert>
using namespace MoonEngine;

GLVertexArrayObject::GLVertexArrayObject()
{
	glGenVertexArrays(1,&_vaoID);
}

GLVertexArrayObject::~GLVertexArrayObject()
{
	LOG(INFO, "Deleting VAO" + std::to_string(_vaoID));
	reset();
}

GLVertexArrayObject::GLVertexArrayObject(GLVertexArrayObject && other):
_vaoID(other.release())
{

}

GLVertexArrayObject & GLVertexArrayObject::operator=(GLVertexArrayObject && other)
{
	reset(other.release());
	return *this;
}

void GLVertexArrayObject::bindVertexBuffer(
	GLuint attribute, 
	const GLBuffer & buffer, 
	GLint size,
	GLenum type, 
	GLboolean normalized,
	GLsizei stride, 
	GLvoid * offset)
{
	assert(buffer.getType() == GL_ARRAY_BUFFER);
	bind();
	buffer.bind();
	glVertexAttribPointer(attribute, size, type, normalized, stride, offset);
	glEnableVertexAttribArray(attribute);
	Unbind();
}

void GLVertexArrayObject::bindElementBuffer(const GLBuffer & buffer)
{
	assert(buffer.getType() == GL_ELEMENT_ARRAY_BUFFER);
	bind();
	buffer.bind();
	Unbind();
}


void GLVertexArrayObject::bind() const
{
	glBindVertexArray(_vaoID);
}

void GLVertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}

GLuint GLVertexArrayObject::release()
{
	GLuint tmp = _vaoID;
	_vaoID = 0;
	return tmp;
}
GLuint GLVertexArrayObject::reset(GLuint newId)
{
	glDeleteVertexArrays(1,&_vaoID);
	_vaoID = newId;
	return _vaoID;
}

