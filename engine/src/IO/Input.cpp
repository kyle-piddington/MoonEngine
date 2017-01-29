#include "IO/Input.h"
#include "Util/MathUtil.h"
#include "KeyboardInputService.h"
#include <cassert>
using namespace MoonEngine;
using namespace MathUtil;
std::shared_ptr<InputService> Input::_service = std::make_shared<KeyboardInputService>();

static float _smoothedHAxis0 = 0;
static float _smoothedVAxis0 = 0;
static float _smoothedHAxis1 = 0;
static float _smoothedVAxis1 = 0;

static float _targVAxis0 = 0;
static float _targHAxis0 = 0;
static float _targHAxis1 = 0;
static float _targVAxis1 = 0;
static float axisSmoothSpeed = 5; //0.1 seconds from start to stop.

void Input::provide(std::shared_ptr<InputService> serv)
{
	_service = serv;
}

float Input::GetAxis(Axis axis)
{
	assert(_service != nullptr);
	float axisVal = _service->getAxisRaw(axis);
	switch(axis)
	{
		case AXIS_HORIZONTAL_0:
			_targHAxis0 = axisVal;
			return _smoothedHAxis0;
			
		case AXIS_HORIZONTAL_1:
			_targHAxis1 = axisVal;
			return _smoothedHAxis1;
			
		case AXIS_VERTICAL_0:
			_targVAxis0 = axisVal;
			return _smoothedVAxis0;
			
		case AXIS_VERTICAL_1:
			_targVAxis1 = axisVal;
			return _smoothedVAxis1;
			
	}
}

bool Input::GetButtonDown(Button b)
{
	return _service->getButtonDown(b);
}

bool Input::GetButton(Button button)
{
	return _service->getButton(button);
}

bool Input::GetButtonUp(Button button)
{
	return _service->getButtonUp(button);
}
void Input::Update(float dt)
{
	//Fast switching between left and right.
	if(sign(_targHAxis0) == -sign(_smoothedHAxis0))
	{
		_smoothedHAxis0 = 0;
	}
	if(sign(_targHAxis1) == -sign(_smoothedHAxis1))
	{
		_smoothedHAxis1 = 0;
	}
	if(sign(_targVAxis0) == -sign(_smoothedVAxis0))
	{
		_smoothedVAxis0 = 0;
	}
	if(sign(_targVAxis1) == -sign(_smoothedVAxis1))
	{
		_smoothedVAxis1 = 0;
	}
	//Linear interpolation to target
	

	_smoothedHAxis0 = moveTowards(_smoothedHAxis0, _targHAxis0, axisSmoothSpeed * dt);
	_smoothedHAxis1 = moveTowards(_smoothedHAxis1, _targHAxis1, axisSmoothSpeed * dt);
	_smoothedVAxis0 = moveTowards(_smoothedVAxis0, _targVAxis0, axisSmoothSpeed * dt);
	_smoothedVAxis1 = moveTowards(_smoothedVAxis1, _targVAxis1, axisSmoothSpeed * dt); 
	
}