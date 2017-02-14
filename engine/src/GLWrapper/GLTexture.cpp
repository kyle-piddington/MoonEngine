#include "GLTexture.h"

using namespace MoonEngine;

GLTexture::GLTexture()
{
    _textureType = GL_TEXTURE_2D;
    _textureWidth = _textureHeight = -1;
}

GLTexture::GLTexture(GLenum textureType)
{
    _textureType = textureType;
    _textureWidth = _textureHeight = -1;
}

GLTexture::~GLTexture()
{
}

bool GLTexture::init(const GLTextureConfiguration & cfg)
{
    
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    //Bind correct texture
    if(cfg.getInputFormat() != GL_RGBA)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);        
    }
    else if(cfg.getInputFormat() == GL_RED && cfg.getDataType() == GL_UNSIGNED_SHORT)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    /* Load the actual texture data */
    glTexImage2D(_textureType, 0, cfg.getInputFormat(), cfg.getWidth(), cfg.getHeight(),
        0, cfg.getOutputFormat(), cfg.getDataType(), data);
    /* Generate mipmap */
    glGenerateMipmap(_textureType);

    /* Let the birds free */
    glBindTexture(_textureType, 0);
    return glGetError() == GL_NO_ERROR;
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

void GLTexture::bind(GLuint unit)
{

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(_textureType, _textureId);
}

void GLTexture::bindSampler(GLuint unit, GLSampler * sampler){
    glBindSampler(unit, sampler->getId());
}

void GLTexture::bindRaw()
{
	glBindTexture(_textureType, _textureId);
}

void GLTexture::unbind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(_textureType, 0);
}