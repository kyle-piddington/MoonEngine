#include "GLBuffer.h"
using namespace MoonEngine;

GLBuffer::GLBuffer()
{
	glGenBuffers(1,&_objectId);
}

GLBuffer::GLBuffer(GLenum target, GLsizeiptr size, const void * data, GLenum usage)
{
	GLBuffer();
	setData(target,size,data,usage);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1,&_objectId);
}

/**
 * Move constructor
 */
GLBuffer::GLBuffer(GLBuffer &&other):
	_objectId(other.release())
{
	
}

/**
 * Assign and move constructor
 */
GLBuffer & GLBuffer::operator=(GLBuffer &&other)
{
	reset(other.release());
	return *this;
}

void GLBuffer::setData(GLenum target, GLsizeiptr size, const void * data, GLenum usage) const
{
	bind(target);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}

void GLBuffer::bind(GLenum target) const
{
	glBindBuffer(target,_objectId);
}

GLuint GLBuffer::getObject() const
{
	return _objectId;
}

GLuint GLBuffer::release()
{
	GLuint retInt = _objectId;
	_objectId = 0;
	return retInt;
}
GLuint GLBuffer::reset(GLuint newObject)
{
	glDeleteBuffers(1,&_objectId);
	_objectId = newObject;
	return _objectId;
}
