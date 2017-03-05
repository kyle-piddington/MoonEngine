#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include <glfw/glfw3.h>
#include <cassert>
#include "util/Logger.h"
#include "GLConstants.h"
#include "GLProgram.h"
#include "thirdparty/imgui/imgui.h"
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
            GLTexture* gl_texture;
            GLuint unit;
            GLenum attachment;
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
        void bindTexture();
        /*Prepare for frame by clear final color texture*/
        void startFrame();

        void bindForGeomPass();
        void bindForStencilPass();
        void bindForLightPass();
        void bindForOutput();

        /*Check the status of the framebuffer*/
        void status();
        /*Bind in @param mode*/
        void bind(GLuint mode);
        
        static void Unbind();

        GLuint getHandle() const;

       



        void UniformTexture(GLProgram* prog, std::string uniformName, std::string textureName);
		/**
		* Specify what part to of the
		* buffer to read from
		* @param name the name of the texture
		*/
		void drawColorAttachments(int size);

        void DBG_DrawToImgui(string guiName);
        

    private:
        GLuint release();

        GLuint reset(GLuint newObject = 0);

        texture_unit getTextureUnit(std::string name) const;
        static int _unitCount;
        int _colorCount;
        int _width;
        int _height;
        GLuint _handle;
        GLenum _framebufferStatus;
        std::unordered_map<std::string, texture_unit> _textureHandles;
    };

}