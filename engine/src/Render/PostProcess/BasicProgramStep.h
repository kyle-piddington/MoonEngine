#pragma once
#include "PostProcessStep.h"
#include "GLWrapper/GLFramebuffer.h"
/**
 * Simple Postprocess step to demonstrate how PPFX works
 * in the context of moonEngine.
 *
 * Applies a program to a texture, and writes
 * it to a framebuffer.
 *
 *
 * Uniforms:
 * 	sampler2d _inputTexture
 */

namespace MoonEngine
{
	class BasicProgramStep : public PostProcessStep
	{
	public:
		BasicProgramStep(std::string progName, std::string inputTextue, GLFramebuffer * framebuffer = nullptr);
	/**
	 * Create and allocate resources needed
	 * to run a post process step
	 * @param window A handle to the GLFW window
	 * @param scene  A handle to the current scene
	 * @param cfg    Additional configuration data
	 */
		void setup(GLFWwindow * window, Scene * scene);

	/**
	 * Render a pass of the post processing
	 * @param scene The scene to render.
	 */
		void render(Scene * scene);
	/**
	 * Deallocate any resources before shutting donw.
	 */
		void shutdown(){}
	private:
		std::string _progName;
		std::string _inputTextureStr;
		GLFramebuffer * fboOut;
		GLTexture * _inputTexture;
		GLProgram * _renderProgram;
	};
}

