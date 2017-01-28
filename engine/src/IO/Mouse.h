#ifndef __MOUSE_H__
#define __MOUSE_H__
#include <GLFW/glfw3.h>
class Mouse
{
public:
   //Get the current X position of the mouse
   static int getX();
   //Get the current yPosition of the mouse
   static int getY();
   //Get the last X position of the mouse
   static int getLastX();
   //Get the last Y position of the mouse
   static int getLastY();
   //Update the mouse coordinates
   
   static double getScrollXAmount();

   static double getScrollYAmount();

   static double getScrollXDelta();

   static double getScrollYDelta();

   static void updateMousePos(int nextX, int nextY);

   //Check to see if a button was clicked
   static bool clicked(int button);

   //Check to see if a button is currently pressed
   static bool pressed(int button);

   //Check to see if a button is currently released
   static bool released(int button);

   //Update the mouse click position
   static void setButtonStatus(int button, int action);

   static void setScrollStatus(double xOffset, double yOffset);
   //Update the mouse button status
   static void update();
private:
   static int x, y, lastX, lastY, bfrX, bfrY;
   static double xScrollOffset, yScrollOffset, deltXOffset, deltYOffset;
   static double deltXBuffer, deltYBuffer;
   static short mouseButtons[GLFW_MOUSE_BUTTON_LAST];
   static short bufferButtons[GLFW_MOUSE_BUTTON_LAST];


};
#endif