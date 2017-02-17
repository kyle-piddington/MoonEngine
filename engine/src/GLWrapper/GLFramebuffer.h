#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include <glfw/glfw3.h>
#include <cassert>
#include "util/Logger.h"
#include "GLConstants.h"
#include "GLProgram.h"
#include <vector>
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
        struct texture_unit {
            GLTexture * gl_texture;
            GLuint unit;
        };

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
        void addTexture(const std::string & textureName, GLTexture & texture, GLenum attachmentInfo);

        void addDepthBuffer();
        /**
         * Todo:
         * Implement a render buffer class
         */

		 /**
		 * Bind the framebuffer
		 * @param mode:
		 * GL_READ_FRAMEBUFFER: bind for reading
		 * GL_DRAW_FRAMEBUFFER: bind for writing
		 * GL_FRAMEBUFFER: bind for both
		 */
        void bind(GLint mode) const;
        
        /**
        * Bind the framebuffer and all textures for output to the screen
        */
        void bindForOutput() const;

        GLuint getObject() const;

        texture_unit getTexture(std::string name) const;
		/**
		* Specify what part to of the
		* buffer to read from
		* @param name the name of the texture
		*/
		void setReadBuffer(std::string name);
		void drawColorAttachments(int size);
		GLuint getAttachmentMode(std::string name) const;

		const std::unordered_map<std::string, texture_unit> & getTextureHandles() const;

        static void Unbind();

        void setDepthStencil(bool depthVal, bool stencilVal);

    private:
        GLuint release();

        GLuint reset(GLuint newObject = 0);

		void bindWithoutComplete(GLuint mode = GL_FRAMEBUFFER) const;
        int _unitCount;
        int _width;
        int _height;
        GLuint _handle;
        GLenum _framebufferStatus;
        std::unordered_map<std::string, texture_unit> _textureHandles;
		std::unordered_map<std::string, GLuint> _textureAttachmentMode;

        //void addRenderbuffer(const GLRenderBuffer & buffer);
    };
}