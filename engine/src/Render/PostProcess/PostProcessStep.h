#pragma once
#include <GLFW/glfw3.h>
#include "Scene/Scene.h"
#include "Loaders/MeshInfo.h"
/**
 * Interface for post processing effects.
 * Post processed effects happen after deferred rendering
 * has completed, and any alpha blending has been applied.
 * 
 */
class PostProcessStep
{
public:
	/**
	 * Create and allocate resources needed
	 * to run a post process step
	 * @param window A handle to the GLFW window
	 * @param scene  A handle to the current scene
	 * @param cfg    Additional configuration data
	 */
	void setup(GLFWwindow * window, Scene * scene, MoonEngineCfg cfg) = 0;
	
	/**
	 * Render a pass of the post processing
	 * @param scene The scene to render.
	 */
	void render(Scene * scene) = 0;
	/**
	 * Deallocate any resources before shutting donw.
	 */
	void shutdown() = 0;
	/**
	 * Convinence method to render to the screen.
	 */
	void renderToScreen()
private:
	MeshInfo * quadInfo;

}