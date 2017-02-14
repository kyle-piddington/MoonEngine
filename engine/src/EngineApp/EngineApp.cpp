#include "EngineApp.h"
#include "IO/GLFWHandler.h"
#include "IO/Keyboard.h"
#include "IO/Input.h"
#include <IO/Mouse.h>
#include "GlobalFuncs/GlobalFuncs.h"
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw_gl3.h"

using namespace MoonEngine;
//Static library
//(Refactor later)
Library EngineApp::AssetLibrary;
Scene * EngineApp::_activeScene = nullptr;
bool EngineApp::assetsLoaded = false;

EngineApp::EngineApp(GLFWwindow * window, MoonEngineCfg config):
        _window(window)
{
    AssetLibrary.Init(config.assetPath);
    assetsLoaded = true;
    glfwSetKeyCallback(window, GLFWHandler::key_callback);
    glfwSetCursorPosCallback(window, GLFWHandler::mousePositionCallback);
    glfwSetMouseButtonCallback(window, GLFWHandler::mouseButtonCallback);
    glfwSetJoystickCallback(GLFWHandler::joystick_callback);
    glfwSetScrollCallback(window, GLFWHandler::scrollWheelCallback);
    glfwSetCharCallback(window, GLFWHandler::characterCallback);

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

void MoonEngine::EngineApp::setMouseMode(int mode)
{
    GLFWHandler::setMouseMode(_window, mode);
}

void handleImguiLock(GLFWwindow * window)
{
    if (Keyboard::key(GLFW_KEY_LEFT_CONTROL) || Mouse::clicked(GLFW_MOUSE_BUTTON_RIGHT))
        {
        toggleImgui();
        if (!isImguiEnabled())
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

    }
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
    ImGui_ImplGlfwGL3_Init(_window, false); //Initialize ImGui
    /* Game loop */
    while (!glfwWindowShouldClose(_window))
    {
        //ImGui implementation
        handleImguiLock(_window);
        
        
        ImGui_ImplGlfwGL3_NewFrame(isImguiEnabled());
        glfwPollEvents();
        GLFWHandler::update();
        Input::Update(dt);

        scene->runUpdate(dt);
        renderer->render(scene);
        newT = (float) glfwGetTime();
        if (isImguiEnabled())
        {
        	//Render materials
            ImGui::Render();            
        }

        glfwSwapBuffers(_window);
        //After scene is completed and rendered, delete any gameObjects
        scene->runDeleteGameObjects();
        dt = newT - t;
        t = newT;
    }
    renderer->shutdown();

}

void EngineApp::initializeComponents(Scene * scene)
{
    for (std::shared_ptr<GameObject> obj : scene->getGameObjects())
    {
        obj->start();
    }
}
