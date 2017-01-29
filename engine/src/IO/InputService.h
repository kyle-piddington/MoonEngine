#pragma once
/**
 * InputSerivce.h:
 * Interface for backing service to perform Input actions.
 */
namespace MoonEngine
{

	enum Axis
	{
		AXIS_HORIZONTAL_0,
		AXIS_VERTICAL_0,
		AXIS_HORIZONTAL_1,
		AXIS_VERTICAL_1
	};

	enum Button
	{
		BUTTON_0,
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_ESC
	};

	class InputService
	{
	public:
	/*
	 * Get the current axis input, sans smoothing
	 * @param  axis the axis to read
	 * @return      Axis value, between -1 and 1
	 */
		virtual float getAxisRaw(Axis axis) = 0;
	/**
	 * Check to see if a button is pressed down.
	 * A button that's been pressed and not released returns true.
	 * @param  button Button to read from
	 * @return        A button
	 */
		virtual bool getButtonDown(Button button) = 0;
	/**
	 * Check to see if a button is allowed up
	 * A button that's just been released 
	 * @param  button The button to check
	 * @return        The button status.
	 */
		virtual bool getButtonUp(Button button) = 0;
	/**
	 * Return if a button was pressed.
	 * If a button is held, this will return false the next frame.
	 * @param  button the button pressed
	 * @return        true if the button was pressed down.
	 */
		virtual bool getButton(Button button) = 0;

	};
}	
