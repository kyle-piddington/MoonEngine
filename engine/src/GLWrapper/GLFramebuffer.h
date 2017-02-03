#pragma once

#include "GLTexture.h"
#include <unordered_map>
/**
 * GLFramebuffer holds on to framebuffer
 * texture information, and allows for easy querying
 * of textures inside.
 * Based off of https://github.com/kyle-piddington/GLFWEngine/blob/master/src/buffers/FrameBuffer.h
 * with a few differences.
 */

namespace MoonEngine
{
    class GLFramebuffer
    {
    public:
        GLFramebuffer(int width, int height);

        ~GLFramebuffer();

        GLFramebuffer(const GLFramebuffer & other) = delete;

        GLFramebuffer & operator=(const GLFramebuffer &) = delete;

        GLFramebuffer(GLFramebuffer && other);

        GLFramebuffer & operator=(GLFramebuffer && other);

        /**
         * Add a texture to the framebuffer
         * the texture will be renderable
         * @param layerName the name of the texture in the framebuffer
         * @param texture   the texture itself.
         */
        void addTexture(const std::string & textureName, const GLTexture & texture, GLenum attachmentInfo);

        /**
         * Todo:
         * Implement a render buffer class
         */

        void bind() const;

        GLuint getObject() const;

        GLuint getTexture(std::string name) const;

        static void Unbind();

    private:
        GLuint release();

        GLuint reset(GLuint newObject = 0);

        void bindWithoutComplete() const;

        int _width;
        int _height;
        GLuint _handle;
        GLenum _framebufferStatus;
        std::unordered_map<std::string, GLuint> _textureHandles;

        //void addRenderbuffer(const GLRenderBuffer & buffer);
    };
}