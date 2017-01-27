#include "KeyboardServiceConfiguration.h"
#include <GLFW/glfw3.h>

using namespace MoonEngine;
KeyboardInputConfiguration::KeyboardInputConfiguration()
{
	BUTTON_0_MAP = GLFW_KEY_SPACE;
	BUTTON_1_MAP = GLFW_KEY_LEFT_SHIFT;
	BUTTON_2_MAP = GLFW_KEY_E;
	BUTTON_3_MAP = GLFW_KEY_F;
	BUTTON_ESC_MAP = GLFW_KEY_ESCAPE;

	AXIS_V_0_POS = GLFW_KEY_W;
	AXIS_V_0_NEG = GLFW_KEY_S;
	AXIS_H_0_POS = GLFW_KEY_D;
	AXIS_H_0_NEG = GLFW_KEY_A;


	AXIS_V_1_POS = GLFW_KEY_UP;
	AXIS_V_1_NEG = GLFW_KEY_DOWN;
	AXIS_H_1_POS = GLFW_KEY_RIGHT;
	AXIS_H_1_NEG = GLFW_KEY_LEFT;
}