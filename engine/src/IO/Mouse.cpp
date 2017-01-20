#include "Mouse.h"
using namespace MoonEngine;
enum Keystatus
{
   PRESS = 1,
   HOLD = 2,
   RELEASE = -1
};

int Mouse::x = -1;
int Mouse::y = -1;
int Mouse::lastX = -1;
int Mouse::lastY = -1;
int Mouse::bfrX = -1;
int Mouse::bfrY = -1;
short Mouse::mouseButtons[GLFW_MOUSE_BUTTON_LAST]={RELEASE};
short Mouse::bufferButtons[GLFW_MOUSE_BUTTON_LAST]={RELEASE};


int Mouse::getX()
{
   return Mouse::x;
}
int Mouse::getY()
{
   return Mouse::y;
}
int Mouse::getLastX()
{
   return Mouse::lastX;
}
int Mouse::getLastY()
{
   return Mouse::lastY;
}

bool Mouse::clicked(int button)
{
   return mouseButtons[button] == PRESS;
}

bool Mouse::pressed(int button)
{

   return mouseButtons[button] >= PRESS;
}
bool Mouse::released(int button)
{
   return mouseButtons[button] == RELEASE;
}
void Mouse::updateMousePos(int nextX, int nextY)
{
   Mouse::bfrX = nextX;
   Mouse::bfrY = nextY;
}

void Mouse::setButtonStatus(int button, int action)
{
   if(action == GLFW_PRESS)
   {
      bufferButtons[button] = PRESS;
   }
   else if(action == GLFW_RELEASE)
   {
      bufferButtons[button] = RELEASE;
   }
}

void Mouse::update()
{
   //If Mouse has never been updated
   if(bfrX != -1 && bfrY != -1)
   {
      if(Mouse::lastX != -1)
         Mouse::lastX = Mouse::x;
      else
         Mouse::lastX = bfrX;
      //If mouse has never been updated
      if(Mouse::lastY != -1)
         Mouse::lastY = Mouse::y;
      else
         Mouse::lastY = bfrY;
      Mouse::x = bfrX;
      Mouse::y = bfrY;
      //Wait for next update
 
   }
   for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
   {
      if(bufferButtons[i] == PRESS)
      {
         if(mouseButtons[i] == PRESS)
         {
            mouseButtons[i] = HOLD;
            bufferButtons[i] = HOLD;
         }
         else
         {
            mouseButtons[i] = PRESS;
         }
      }
      else if(bufferButtons[i] == RELEASE)
      {
         mouseButtons[i] = RELEASE;
      }

   }
}

