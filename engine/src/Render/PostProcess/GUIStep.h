#pragma once
#include "PostProcessStep.h"
#include "GLWrapper/GLFramebuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Component/Components.h"

/**
 * Gui postprocess. Writes quads directly in front of camera.
 */

namespace MoonEngine
{
	class GUIStep : public PostProcessStep
	{
	public:
        GUIStep(int width, int height);
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
		GLFramebuffer _fbo;

		GLTexture * _compositeTexture;
		GLProgram * _renderProgram;

        int _width;
        int _height;

        Camera* _mainCamera;
    };
}

