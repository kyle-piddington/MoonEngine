#pragma once
#include "PostProcessStep.h"
#include "GLWrapper/GLFramebuffer.h"
#include "GameObject/GameObject.h"

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
	class GodrayStep : public PostProcessStep
	{
	public:
		GodrayStep(std::string progName);
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

		void drawSun();
		std::string _progName;
		GLProgram * _godrayProgram;
		GLProgram * _maskProgram;
		GLTexture * _maskTexture;
		std::shared_ptr<GLFramebuffer> maskFramebuffer;
		std::shared_ptr<GLFramebuffer> compositionFramebuffer;
		
		std::shared_ptr<GameObject> sunObject;
		std::shared_ptr<GameObject> _mainCamera;


	};
}

