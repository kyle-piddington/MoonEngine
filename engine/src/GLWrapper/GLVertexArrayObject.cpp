#include "GLVertexArrayObject.h"
#include "Util/Logger.h"
#include <cassert>
#include <string>
#include "GLUtil/GL_LOG.h"

using namespace MoonEngine;

GLVertexArrayObject::GLVertexArrayObject()
{
    glGenVertexArrays(1, &_vaoID);
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

void GLVertexArrayObject::bindVertexBuffer(GLuint attribute, const GLBuffer & buffer, GLint size,
    GLenum type, GLboolean normalized, GLsizei stride, GLvoid * offset)
{
    assert(buffer.getType() == GL_ARRAY_BUFFER);
    bind();
    buffer.bind();
    if (attribute == GL_VERTEX_INSTANCE_MATRIX_ATTRIBUTE)
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid *)(uintptr_t) 0);
        glEnableVertexAttribArray(attribute + 1);
        glVertexAttribPointer(attribute + 1, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid *)(uintptr_t) (stride));
        glEnableVertexAttribArray(attribute + 2);
        glVertexAttribPointer(attribute + 2, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid *)(uintptr_t) (2 * stride));
        glEnableVertexAttribArray(attribute + 3);
        glVertexAttribPointer(attribute + 3, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid *)(uintptr_t) (3 * stride));

        glVertexAttribDivisor(attribute, 1);
        glVertexAttribDivisor(attribute + 1, 1);
        glVertexAttribDivisor(attribute + 2, 1);
        glVertexAttribDivisor(attribute + 3, 1);
    }
    else
    {
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, size, type, normalized, stride, offset);
    }
    LOG_GL(__FILE__, __LINE__);

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
    glDeleteVertexArrays(1, &_vaoID);
    _vaoID = newId;
    return _vaoID;
}

