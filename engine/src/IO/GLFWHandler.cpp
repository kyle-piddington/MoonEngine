#include "GLFWHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Util/Logger.h"
#include "Controller.h"
#include "ControllerInputService.h"
#include "KeyboardInputService.h"
#include "Input.h"
using namespace MoonEngine;
void  GLFWHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GL_TRUE);
   }
   Keyboard::setKeyStatus(key,action);

   
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
   if(Controller::IsConnected())
   {
      Controller::Update();    
   }

}

void GLFWHandler::joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED)
    {
        if(!Controller::IsConnected())
        {
          LOG(GAME, std::to_string(joy) + " was connected");
          Controller::Connect(joy);
          Input::provide(std::make_shared<ControllerInputService>());

        }
        // The joystick was connected
        // Test to see if joystick will be good 
        // enough for game (2 axes)
    }
    else if (event == GLFW_DISCONNECTED)
    {
        LOG(GAME, std::to_string(joy) + " was disconnected");
        Controller::Disconnect();
        Input::provide(std::make_shared<KeyboardInputService>());

        // The joystick was disconnected
    }
}

void GLFWHandler::Start()
{
  //Check for controller input
  int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
  if(present == GLFW_TRUE)
  {
    Controller::Connect(GLFW_JOYSTICK_1);
    Input::provide(std::make_shared<ControllerInputService>());

  }
}
