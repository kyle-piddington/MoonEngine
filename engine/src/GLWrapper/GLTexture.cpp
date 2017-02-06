#include "GLTexture.h"

using namespace MoonEngine;

GLTexture::GLTexture(GLuint unit)
{
    _unit = GL_TEXTURE0 + unit;
    _textureType = GL_TEXTURE_2D;
    _textureWidth = _textureHeight = -1;
}

GLTexture::GLTexture(GLuint unit, GLenum textureType)
{
    _unit = unit;
    _textureType = textureType;
    _textureWidth = _textureHeight = -1;
}

GLTexture::~GLTexture()
{
}

bool GLTexture::init(const GLTextureConfiguration & cfg)
{
    init(nullptr, cfg);
    _textureWidth = cfg.getWidth();
    _textureHeight = cfg.getHeight();

    /* Generate a texture buffer object */
    glGenTextures(1, &_textureId);
    /* Bind the current texture to be the texture object */
    glBindTexture(_textureType, _textureId);
    /* Load the actual texture data */
    // How do we support both RGB & RGBA?
    glTexImage2D(_textureType, 0, cfg.getInputFormat(), cfg.getWidth(), cfg.getHeight(),
        0, cfg.getOutputFormat(), cfg.getDataType(), NULL);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(_textureType, 0);
    return glGetError() == GL_NO_ERROR;

}

/* We use init to ensure that loading an image didn't fail */
bool GLTexture::init(void * data, const GLTextureConfiguration & cfg)
{
    _textureWidth = cfg.getWidth();
    _textureHeight = cfg.getHeight();
    /* Generate a texture buffer object */
    glGenTextures(1, &_textureId);
    /* Bind the current texture to be the texture object */
    glBindTexture(_textureType, _textureId);
    /* Load the actual texture data */
    glTexImage2D(_textureType, 0, cfg.getInputFormat(), cfg.getWidth(), cfg.getHeight(),
        0, cfg.getOutputFormat(), cfg.getDataType(), data);
    /* Generate mipmap */
    glGenerateMipmap(_textureType);

    /* Let the birds free */
    glBindTexture(_textureType, 0);
    return glGetError() == GL_NO_ERROR;
}

GLuint GLTexture::getUnit() const
{
    return _unit;
}

GLint GLTexture::getTextureId() const
{
    return _textureId;
}

GLenum GLTexture::getTextureType() const
{
    return _textureType;
}

int GLTexture::getWidth() const
{
    return _textureWidth;
}

int GLTexture::getHeight() const
{
    return _textureHeight;
}

void GLTexture::bind()
{
    glActiveTexture(_unit);
    glBindTexture(_textureType, _textureId);
}

void GLTexture::bindRaw()
{
	glBindTexture(_textureType, _textureId);
}

void GLTexture::unbind()
{
    glActiveTexture(_unit);
    glBindTexture(_textureType, 0);
}

void GLTexture::bindSampler(GLSampler * sampler)
{
    glBindSampler(_unit, sampler->getId());
}

void GLTexture::unbindSampler()
{
    glBindSampler(_unit, 0);

}