#pragma once

#include "IO/InputService.h"
/*
	KeyboardServiceConfiguration
	Class for modifying keyboard inputs on the fly.
	Provides key-mapping for input serivce	
 */
namespace MoonEngine
{
    struct InputConfiguration
    {
        static InputConfiguration DefaultKeyboardConfiguration();

        static InputConfiguration DefaultControllerConfiguration();

        int BUTTON_0_MAP;
        int BUTTON_1_MAP;
        int BUTTON_2_MAP;
        int BUTTON_3_MAP;
        int BUTTON_ESC_MAP;

        int AXIS_V_0_POS;
        int AXIS_V_0_NEG;
        int AXIS_H_0_POS;
        int AXIS_H_0_NEG;


        int AXIS_V_1_POS;
        int AXIS_V_1_NEG;
        int AXIS_H_1_POS;
        int AXIS_H_1_NEG;

    };
}
