#include "InputConfiguration.h"
#include <GLFW/glfw3.h>

using namespace MoonEngine;

InputConfiguration InputConfiguration::DefaultKeyboardConfiguration()
{
    InputConfiguration cfg;
    cfg.BUTTON_0_MAP = GLFW_KEY_SPACE; //Jump
    cfg.BUTTON_1_MAP = GLFW_KEY_LEFT_SHIFT; //Sprint
    cfg.BUTTON_2_MAP = GLFW_KEY_E; //Center
    cfg.BUTTON_3_MAP = GLFW_KEY_F; //Pause
    cfg.BUTTON_ESC_MAP = GLFW_KEY_ESCAPE;

    cfg.AXIS_V_0_POS = GLFW_KEY_W;
    cfg.AXIS_V_0_NEG = GLFW_KEY_S;
    cfg.AXIS_H_0_POS = GLFW_KEY_D;
    cfg.AXIS_H_0_NEG = GLFW_KEY_A;


    cfg.AXIS_V_1_POS = GLFW_KEY_UP;
    cfg.AXIS_V_1_NEG = GLFW_KEY_DOWN;
    cfg.AXIS_H_1_POS = GLFW_KEY_RIGHT;
    cfg.AXIS_H_1_NEG = GLFW_KEY_LEFT;
    return cfg;
}

InputConfiguration InputConfiguration::DefaultControllerConfiguration()
{
    InputConfiguration cfg;
    cfg.BUTTON_0_MAP = 11; //Jump
    cfg.BUTTON_1_MAP = 12; //Sprint
    cfg.BUTTON_2_MAP = 7; //center
    cfg.BUTTON_3_MAP = 7; //pause (Click RThumbStick)
    cfg.BUTTON_ESC_MAP = 5; //Back

    cfg.AXIS_V_0_POS = 1;
    cfg.AXIS_V_0_NEG = 1;
    cfg.AXIS_H_0_POS = 0;
    cfg.AXIS_H_0_NEG = 0;


    cfg.AXIS_V_1_POS = 3;
    cfg.AXIS_V_1_NEG = 3;
    cfg.AXIS_H_1_POS = 2;
    cfg.AXIS_H_1_NEG = 2;
    return cfg;
}