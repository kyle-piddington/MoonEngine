#pragma once

#include <string>
#include "GLSampler.h"
#include "OpenGL.h"
#include "GLTextureConfiguration.h"

namespace MoonEngine
{
    class GLTexture
    {
    public:

        /**
         * Create a texture object
         */
        GLTexture(GLuint handle);

        /**
         * Create a texture object
         */
        GLTexture(GLuint handle, GLenum textureType);

        /**
         * Delete a program object.
         */
        ~GLTexture();

        /**
         * Copy constructor(s) is disallowed for textures.
         */
        GLTexture(const GLTexture & other) = delete;
        GLTexture &operator=(const GLTexture &) = delete;

        /**
        Move Constructor(s)
         */
        GLTexture(GLTexture &&other);
        GLTexture &operator=(GLTexture &&other);

        /**
         * Initialize texture with data and a configuration
         * @param  data the data of the texture
         * @param  cfg  a configuration file describing the texture
         * @return      true if initialization is successful.
         */
        bool init(void * data, const GLTextureConfiguration & cfg);

        GLuint getUnit();

        GLint getTextureId();

        /**
         * Bind the texture
         */
        void bind();

        /**
         * Bind the texture
         */
        void unbind();

        void bindSampler(GLSampler * sampler);
        void unbindSampler();

    private:
        GLuint _textureId;
        GLenum _textureType;

        GLuint _unit;
    };
}