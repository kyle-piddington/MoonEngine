#pragma once

#include "InputConfiguration.h"
#include "IO/InputService.h"
/*
	KeyboardInputSerivce
	Provides wrapping around input service for a keyboard input.
	Configured via a KeyboardServiceConfiguration
	Uses Keyboard classes as backing.
 */

namespace MoonEngine
{

    class KeyboardInputService: public InputService
    {
    public:
        KeyboardInputService();

        KeyboardInputService(InputConfiguration cfg);

        float getAxisRaw(Axis axis);

        bool getButton(Button button);

        bool getButtonUp(Button button);

        bool getButtonDown(Button button);

    private:
        int getButtonMapping(Button button);

        InputConfiguration inputConfig;
    };
}
