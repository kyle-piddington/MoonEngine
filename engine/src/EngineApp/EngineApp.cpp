#include "EngineApp.h"
#include "IO/GLFWHandler.h"
#include "IO/Keyboard.h"
#include "GlobalFuncs/Instantiate.h"
using namespace MoonEngine;
//Static library
//(Refactor later)
Library EngineApp::AssetLibrary;
bool EngineApp::assetsLoaded = false;

EngineApp::EngineApp(GLFWwindow * window, MoonEngineCfg config):
window(window)
{
	AssetLibrary.Init(config.assetPath);
	assetsLoaded = true;
	glfwSetKeyCallback(window, GLFWHandler::key_callback);
   	glfwSetCursorPosCallback(window, GLFWHandler::mousePositionCallback);
   	glfwSetMouseButtonCallback(window, GLFWHandler::mouseButtonCallback);
	//Other app setup code, install callbacks etc.
	
}

EngineApp::~EngineApp()
{


}

Library EngineApp::GetAssetLibrary()
{
	assert(assetsLoaded);
	return AssetLibrary;
}

void EngineApp::run(Scene * scene, I_Renderer * renderer)
{
	//Set the global active scene to this one.
	SetActiveScene(scene);
	initializeComponents(scene);
	float newT, t = (float) glfwGetTime();
	float dt = 0;
	renderer->setup(scene);
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        GLFWHandler::update();
        
		scene->runCollisionUpdate();
		scene->runUpdate(dt);
		renderer->render(scene);
		newT = (float) glfwGetTime();
		glfwSwapBuffers(window);
		dt =  newT - t;
		t = newT;
	}
	renderer->shutdown();

}

void EngineApp::initializeComponents(Scene * scene)
{
	for(std::shared_ptr<GameObject>  obj : scene->getGameObjects())
	{
		obj->start();
	}
}

