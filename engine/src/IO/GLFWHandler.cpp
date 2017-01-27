#include "GLFWHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
using namespace MoonEngine;
void  GLFWHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
   Keyboard::setKeyStatus(key,action);
   //Can do more loggging or updating here.
}

void GLFWHandler::mousePositionCallback(GLFWwindow * window, double x, double y)
{
	Mouse::updateMousePos((int)x, (int)y);
}

void GLFWHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
   Mouse::setButtonStatus(button,action);
}

void GLFWHandler::update()
{
   Keyboard::update();
   Mouse::update();
}

void GLFWHandler::joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED)
    {
        // The joystick was connected
    }
    else if (event == GLFW_DISCONNECTED)
    {
        // The joystick was disconnected
    }
}
