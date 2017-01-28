#include "IO/KeyboardInputService.h"
#include "IO/Keyboard.h"

using namespace MoonEngine;
KeyboardInputService::KeyboardInputService():
inputConfig(InputConfiguration::DefaultKeyboardConfiguration())
{

}

float KeyboardInputService::getAxisRaw(Axis axis)
{
	bool isUpPressed;
	bool isDownPressed;
	bool isRightPressed;
	bool isLeftPressed;
	switch(axis)
	{
		case AXIS_VERTICAL_0:
			isUpPressed = Keyboard::isKeyDown(inputConfig.AXIS_V_0_POS);
			isDownPressed = Keyboard::isKeyDown(inputConfig.AXIS_V_0_NEG);
			return -1*(int)isUpPressed + (int)isDownPressed;
		case AXIS_VERTICAL_1:
			isUpPressed = Keyboard::isKeyDown(inputConfig.AXIS_V_1_POS);
			isDownPressed = Keyboard::isKeyDown(inputConfig.AXIS_V_1_NEG);
			return -1*(int)isUpPressed + (int)isDownPressed;

		case AXIS_HORIZONTAL_0:
			isLeftPressed = Keyboard::isKeyDown(inputConfig.AXIS_H_0_POS);
			isRightPressed = Keyboard::isKeyDown(inputConfig.AXIS_H_0_NEG);
			return -1*(int)isLeftPressed + (int)isRightPressed;

		case AXIS_HORIZONTAL_1:
			isLeftPressed = Keyboard::isKeyDown(inputConfig.AXIS_H_1_POS);
			isRightPressed = Keyboard::isKeyDown(inputConfig.AXIS_H_1_NEG);
			return -1*(int)isLeftPressed + (int)isRightPressed;
	}
}

bool KeyboardInputService::getButton(Button button)
{
	return Keyboard::isKeyDown(getButtonMapping(button));

}

bool KeyboardInputService::getButtonUp(Button button)
{
	return Keyboard::isKeyUp(getButtonMapping(button));

}


bool KeyboardInputService::getButtonDown(Button button)
{
	return Keyboard::key(getButtonMapping(button));

}


int KeyboardInputService::getButtonMapping(Button button)
{
	switch(button)
	{
		case BUTTON_0:
			return inputConfig.BUTTON_0_MAP;
			
		case BUTTON_1:
			return inputConfig.BUTTON_1_MAP;
			
		case BUTTON_2:
			return inputConfig.BUTTON_2_MAP;
			
		case BUTTON_3:
			
			return inputConfig.BUTTON_3_MAP;
		case BUTTON_ESC:
			return inputConfig.BUTTON_ESC_MAP;
			
	}
}