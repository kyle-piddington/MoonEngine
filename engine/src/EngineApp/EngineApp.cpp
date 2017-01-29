#include "EngineApp.h"
#include "IO/GLFWHandler.h"
#include "IO/Keyboard.h"
#include "IO/Input.h"
#include "GlobalFuncs/Instantiate.h"
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw_gl3.h"
using namespace MoonEngine;
//Static library
//(Refactor later)
Library EngineApp::AssetLibrary;
Scene * EngineApp::activeScene = nullptr;
bool EngineApp::assetsLoaded = false;

EngineApp::EngineApp(GLFWwindow * window, MoonEngineCfg config):
window(window)
{
	AssetLibrary.Init(config.assetPath);
	assetsLoaded = true;
	glfwSetKeyCallback(window, GLFWHandler::key_callback);
   	glfwSetCursorPosCallback(window, GLFWHandler::mousePositionCallback);
   	glfwSetMouseButtonCallback(window, GLFWHandler::mouseButtonCallback);
	glfwSetJoystickCallback(GLFWHandler::joystick_callback);
	glfwSetScrollCallback(window, GLFWHandler::scrollWheelCallback);
    glfwSetCharCallback(window,GLFWHandler::characterCallback);

	GLFWHandler::Start();
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
	scene->start();
	ImGui_ImplGlfwGL3_Init(window,false); //Initialize ImGui

	while(!glfwWindowShouldClose(window))
	{
		//ImGui implementation
		
		ImGui_ImplGlfwGL3_NewFrame();
    
		glfwPollEvents();
        GLFWHandler::update();
        Input::Update(dt);
		scene->runCollisionUpdate();
		scene->runUpdate(dt);
		renderer->render(scene);
		newT = (float) glfwGetTime();

		ImGui::Render();
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



