#pragma once

#include <string>
#include "GLSampler.h"
#include "OpenGL.h"
#include "GLUtil/GLTextureConfiguration.h"

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

        GLTexture & operator=(const GLTexture &) = delete;

        /**
        Move Constructor(s)
         */
        GLTexture(GLTexture && other);

        GLTexture & operator=(GLTexture && other);

        /**
         * Initialize texture with data and a configuration
         * @param  data the data of the texture
         * @param  cfg  a configuration file describing the texture
         * @return      true if initialization is successful.
         */
        bool init(void * data, const GLTextureConfiguration & cfg);

        bool init(const GLTextureConfiguration & cfg);

        GLuint getUnit() const;

        GLint getTextureId() const;

        GLenum getTextureType() const;

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

        int getWidth() const;

        int getHeight() const;

    private:
        GLuint _textureId;
        GLenum _textureType;
        GLenum _framebufferStatus;

        int _textureWidth;
        int _textureHeight;
        GLuint _unit;
    };
}