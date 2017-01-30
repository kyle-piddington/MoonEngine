#include "GLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb_image.h>

using namespace MoonEngine;

GLTexture::GLTexture(GLuint unit)
{
    _unit = unit;
    _textureType = GL_TEXTURE_2D;
}

GLTexture::GLTexture(GLuint unit, GLenum textureType) {
    _unit = unit;
    _textureType = textureType;
}

GLTexture::~GLTexture()
{
}

/* We use init to ensure that loading an image didn't fail */
bool GLTexture::init(std::string textureName) {
    int width, height, ncomps;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(textureName.c_str(), &width, &height, &ncomps, 0);
    /* File not found */
    if (!data) {
        return false;
    }

    /* Generate a texture buffer object */
    glGenTextures(1, &_textureId);
    /* Bind the current texture to be the texture object */
    glBindTexture(_textureType, _textureId);
    /* Load the actual texture data */
    glTexImage2D(_textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    /* Generate mipmap */
    glGenerateMipmap(_textureType);

    /* Let the birds free */
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return true;
}

void GLTexture::bind()
{
    glActiveTexture(_unit);
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


