#pragma once

#include <GLFW/glfw3.h>
#include <string>
/**
 * Includes GLFW callback functions, forwards events to other parts of app
 */
namespace MoonEngine
{
    struct GLFWHandler
    {
        /**
         * Provide a callback for handling key inputs.
         * @param  window  the sending window of this event.
         * @param  key     the GLFW_Key code
         * @param   scancode  the ASCII code
         * @param   action    PRESS, RELEASE, or REPEAT.
         * @param   mode      the status of Ctrl, Shift, and alt.
         */
        static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

        /**
         * Forward any mouse position changes to the app
         * @param  window  the sending window of this event
         * @param  x   the current x position of the mouse
         * @param  y   the current y position of the mouse.
         */
        static void mousePositionCallback(GLFWwindow * window, double x, double y);

        /**
         * Forward joystic connection events
         * @param joy   the joystick
         * @param event a connection state
         */
        static void joystick_callback(int joy, int event);

        /**
       * Forward any scroll wheel changes to the application
       * @param window  the sending window
       * @param xoffset horizontal scrolling
       * @param yoffset vertical scrolling
       */
        static void scrollWheelCallback(GLFWwindow * window, double xoffset, double yoffset);

        /**
         * Forward any character presses directly to the keyboard
         */
        static void characterCallback(GLFWwindow * window, unsigned int c);

        static void setMouseMode(GLFWwindow * window, int mode);

        /**
         * Forward any mouse input events to the rest of the app
         * @param  window  the sending window of this event
         * @param  button  the mouse button pressed.
         * @param  action  PRESS or RELEASE events
         * @param  mods  any modification keys that were pressed.
         */
        static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

        /**
         * Update any input handlers. (Mouse/Keyboard)
         */
        static void update();

        static void Start();

    };
}
