#pragma once
#include "GLTexture.h"
/**
 * GLFramebuffer holds on to framebuffer
 * texture information, and allows for easy querying
 * of textures inside.
 * Based off of https://github.com/kyle-piddington/GLFWEngine/blob/master/src/buffers/FrameBuffer.h
 * with a few differences.
 */

namespace MoonEngine
{
	class GLFrameBuffer
	{
	public:
		GLFrameBuffer(int width, int height);
		~GLFrameBuffer();

		GLFrameBuffer(const GLFrameBuffer & other) = delete;
		GLFrameBuffer & operator=(const GLFrameBuffer & ) = delete;

		GLFrameBuffer(GLFrameBuffer && other);
		GLFrameBuffer &operator=(GLFrameBuffer &&other);
		/**
		 * Add a texture to the framebuffer
		 * the texture will be renderable
		 * @param layerName the name of the texture in the framebuffer
		 * @param texture   the texture itself.
		 */
		void addTexture(std::string & layerName, const GLTexture & texture);
		/**
		 * Todo:
		 * Implement a render buffer class
		 */
		
	private:
		int _width;
		int _height;


		//void addRenderbuffer(const GLRenderBuffer & buffer);
		//
	};
}