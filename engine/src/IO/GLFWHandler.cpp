#include "GLFWHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Util/Logger.h"
#include "Controller.h"
#include "ControllerInputService.h"
#include "KeyboardInputService.h"
#include "Input.h"

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw_gl3.h"

using namespace MoonEngine;

void GLFWHandler::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    /**
    * Allow Imgui to consume the event
    */
    if (ImGui::IsWindowFocused() || ImGui::IsMouseHoveringAnyWindow())
    {
        ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
    }
    else
    {
        Keyboard::setKeyStatus(key, action);
    }
}

void GLFWHandler::mousePositionCallback(GLFWwindow * window, double x, double y)
{
    Mouse::updateMousePos(x, y);
}

void GLFWHandler::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    //Imgui event capture
    //Can't select a window without actually hovering

    if (ImGui::IsWindowFocused() || ImGui::IsMouseHoveringAnyWindow())
    {
        ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
    }
    else
    {
        Mouse::setButtonStatus(button, action);

    }
}

void GLFWHandler::scrollWheelCallback(GLFWwindow * window, double xOffset, double yOffset)
{
    if (ImGui::IsWindowFocused() || ImGui::IsMouseHoveringAnyWindow())
    {
        ImGui_ImplGlfwGL3_ScrollCallback(window, xOffset, yOffset);
    }
    else
    {
        Mouse::setScrollStatus(xOffset, yOffset);
    }
}

void GLFWHandler::characterCallback(GLFWwindow * window, unsigned int c)
{
    ImGui_ImplGlfwGL3_CharCallback(window, c);
}

void GLFWHandler::setMouseMode(GLFWwindow * window, int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void GLFWHandler::update()
{
    Keyboard::update();
    Mouse::update();
    if (Controller::IsConnected())
    {
        Controller::Update();
    }
}

void GLFWHandler::joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED)
    {
        if (!Controller::IsConnected())
        {
            LOG(GAME, std::to_string(joy) + " was connected");
            Controller::Connect(joy);
            Input::provide(std::make_shared<ControllerInputService>());
        }
    }
    else if (event == GLFW_DISCONNECTED)
    {
        LOG(GAME, std::to_string(joy) + " was disconnected");
        Controller::Disconnect();
        Input::provide(std::make_shared<KeyboardInputService>());
    }
}

void GLFWHandler::Start()
{
    //Check for controller input
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (present == GLFW_TRUE)
    {
        Controller::Connect(GLFW_JOYSTICK_1);
        Input::provide(std::make_shared<ControllerInputService>());
    }
}