#pragma once

#include <string>
#include "GLSampler.h"
#include "OpenGL.h"
#include "GLUtil/GLTextureConfiguration.h"
#include "GLUtil/GL_LOG.h"

namespace MoonEngine
{
    class GLTexture
    {
    public:

        /**
         * Create a texture object
         */
        GLTexture();

        /**
         * Create a texture object
         */
        GLTexture(GLenum textureType);

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

        GLint getTextureId() const;

        GLenum getTextureType() const;

        /**
         * Bind the texture
         */
        void bind(GLuint unit);

		void bindRaw();

        void bindSampler(GLuint unit, GLSampler * sampler);

        /**
         * Bind the texture
         */
        void unbind(GLuint unit);

        int getWidth() const;

        int getHeight() const;

    private:
        GLuint _textureId;
        GLenum _textureType;
        GLenum _framebufferStatus;

        int _textureWidth;
        int _textureHeight;
    };
}