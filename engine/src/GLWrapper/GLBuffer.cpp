#include "GLBuffer.h"
#include "Util/Logger.h"
#include <string>

using namespace MoonEngine;

GLBuffer::GLBuffer(GLenum target):
        _type(target)
{
    glGenBuffers(1, &_objectId);
}

GLBuffer::GLBuffer(GLenum target, GLsizeiptr size, const void * data, GLenum usage):
        _type(target)
{
    glGenBuffers(1, &_objectId);
    setData(size, data, usage);
}

GLBuffer::~GLBuffer()
{
    LOG(INFO, "Deleting Buffer " + std::to_string(_objectId));
    reset();
}

/**
 * Move constructor
 */
GLBuffer::GLBuffer(GLBuffer && other):
        _type(other._type),
        _objectId(other.release())
{

}

/**
 * Assign and move constructor
 */
GLBuffer & GLBuffer::operator=(GLBuffer && other)
{
    reset(other.release());
    _type = other._type;
    return *this;
}

void GLBuffer::setData(GLsizeiptr size, const void * data, GLenum usage) const
{
    bind();
    glBufferData(_type, size, data, usage);
    glBindBuffer(_type, 0);
}

void GLBuffer::bind() const
{
    glBindBuffer(_type, _objectId);
}

GLuint GLBuffer::getObject() const
{
    return _objectId;
}

GLenum GLBuffer::getType() const
{
    return _type;
}

GLuint GLBuffer::release()
{
    GLuint retInt = _objectId;
    _objectId = 0;
    return retInt;
}

GLuint GLBuffer::reset(GLuint newObject)
{
    glDeleteBuffers(1, &_objectId);
    _objectId = newObject;
    return _objectId;
}
