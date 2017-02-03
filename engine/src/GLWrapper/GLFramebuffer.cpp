#include "GLFramebuffer.h"
#include <glfw/glfw3.h>
#include <cassert>
#include "util/Logger.h"

using namespace MoonEngine;

GLFramebuffer::GLFramebuffer(int width, int height):
        _width(width),
        _height(height),
        _textureHandles(std::unordered_map<std::string, GLuint>()),
        _framebufferStatus(GL_FRAMEBUFFER_UNDEFINED)
{
    glGenFramebuffers(1, &_handle);
}

GLFramebuffer::~GLFramebuffer()
{
    LOG(INFO, "Deleting Framebuffer " + std::to_string(_handle));
    glDeleteFramebuffers(1, &_handle);
}

GLFramebuffer::GLFramebuffer(GLFramebuffer && other):
        _width(other._width),
        _height(other._height),
        _textureHandles(other._textureHandles),
        _handle(other.release()),
        _framebufferStatus(other._framebufferStatus)
{

}

GLFramebuffer & GLFramebuffer::operator=(GLFramebuffer && other)
{
    _width = other._width;
    _height = other._height;
    _textureHandles = other._textureHandles;
    reset(other.release());
    return *this;
}

void GLFramebuffer::bind() const
{
    if (_framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (_framebufferStatus)
        {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                LOG(ERROR, "Framebuffer not complete, incomplete attachment");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                LOG(ERROR, "Framebuffer not complete, No textures attached");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                LOG(ERROR, "Framebuffer not complete, not supported by openGL version");
                break;
            default:
                LOG(ERROR, "FrameBuffer not complete... " + std::to_string(_framebufferStatus));

        }
    }
    assert(_framebufferStatus == GL_FRAMEBUFFER_COMPLETE);
    bindWithoutComplete();

}

GLuint MoonEngine::GLFramebuffer::getObject() const
{
    return _handle;
}

void GLFramebuffer::bindWithoutComplete() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}

void GLFramebuffer::addTexture(const std::string & textureName, const GLTexture & texture, GLenum attachmentInfo)
{
    assert(texture.getWidth() == _width && texture.getHeight() == _height);
    bindWithoutComplete();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentInfo, GL_TEXTURE_2D, texture.getTextureId(), 0);
    _textureHandles[textureName] = texture.getTextureId();
    _framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    Unbind();
}


GLuint GLFramebuffer::release()
{
    GLuint tmp = _handle;
    _handle = 0;
    return tmp;
}

GLuint GLFramebuffer::reset(GLuint newObject)
{
    glDeleteFramebuffers(1, &_handle);
    _handle = newObject;
    return _handle;
}

void GLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLFramebuffer::getTexture(std::string name) const
{
    auto it = _textureHandles.find(name);
    if (it == _textureHandles.end())
    {
        assert(!"Could not find texture in framebuffer");
    }
    return it->second;
}

