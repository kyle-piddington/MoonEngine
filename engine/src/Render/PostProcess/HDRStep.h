#pragma once
#include "PostProcessStep.h"
#include "GLWrapper/GLFramebuffer.h"

namespace MoonEngine
{
	class HDRStep : public PostProcessStep
	{
	public:
		HDRStep(std::string progName);
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
		void shutdown() {}
	private:
		std::string _progName;

		GLTexture * _inputTexture;
		GLProgram * _renderProgram;
	};
}