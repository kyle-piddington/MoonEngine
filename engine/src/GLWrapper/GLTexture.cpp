#include "GLTexture.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include <thirdparty/stb_image.h>
// #include <iostream>

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
bool GLTexture::init(void * data,const  GLTextureConfiguration & cfg) {
    // string texture_file = textureName + ".png";
    // std::cout << texture_file << endl;

    // int width, height, ncomps;

    // stbi_set_flip_vertically_on_load(true);
    // unsigned char *data = stbi_load(texture_file.c_str(), &width, &height, &ncomps, 0);
    // /* File not found */
    // if (!data) {
    //     std::cout << "NOt found";
    //     return false;
    // }

    /* Generate a texture buffer object */
    glGenTextures(1, &_textureId);
    /* Bind the current texture to be the texture object */
    glBindTexture(_textureType, _textureId);
    /* Load the actual texture data */
    // How do we support both RGB & RGBA?
    glTexImage2D(_textureType, 0, cfg.getInputFormat(), cfg.getWidth(), cfg.getHeight(), 0, cfg.getOutputFormat(), cfg.getDataType(), data);
    /* Generate mipmap */
    glGenerateMipmap(_textureType);

    /* Let the birds free */
    glBindTexture(_textureType, 0);
    

    return glGetError() == false;
}

GLuint GLTexture::getUnit() {
    return _unit;
}

GLint GLTexture::getTextureId()
{
    return _textureId;
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


