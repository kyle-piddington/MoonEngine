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
   Mouse::updateMousePos(x,y);
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
