#include "ControllerInputService.h"
#include "Controller.h"
using namespace MoonEngine;

ControllerInputService::ControllerInputService():
	inputConfig(InputConfiguration::DefaultControllerConfiguration())
	{

	}
ControllerInputService::ControllerInputService(InputConfiguration cfg):
	inputConfig(cfg)
	{

	}

float ControllerInputService::getAxisRaw(Axis axis)
{
	
	switch(axis)
	{
		case AXIS_VERTICAL_0:
			return Controller::GetAxis(inputConfig.AXIS_V_0_POS);
		case AXIS_VERTICAL_1:
			return Controller::GetAxis(inputConfig.AXIS_V_1_POS);
		case AXIS_HORIZONTAL_0:
			return -Controller::GetAxis(inputConfig.AXIS_H_0_POS);
		case AXIS_HORIZONTAL_1:
			return -Controller::GetAxis(inputConfig.AXIS_H_1_POS);
	}
}

bool ControllerInputService::getButton(Button button)
{
	return Controller::Button(getButtonMapping(button));
}

bool ControllerInputService::getButtonUp(Button button)
{
	return Controller::IsButtonUp(getButtonMapping(button));
}

bool ControllerInputService::getButtonDown(Button button)
{
	return Controller::IsButtonDown(getButtonMapping(button));
}


int ControllerInputService::getButtonMapping(Button button)
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