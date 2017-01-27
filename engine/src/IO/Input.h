#pragma once
#include "InputService.h"
#include <memory>
/**
 * Input.h
 * Handles generic input for both camera and keyboard.
 * Supply with a service to bind to a class
 * use Input::GetAxis() to register WASD or arrow key events,
 * Input::GetButton() for face keys and other events.
 */

namespace MoonEngine
{

	class Input
	{
		public:

		static void provide(std::shared_ptr<InputService> service);
	/**
	 * Get the current axis input, with small smoothing values.
	 * @param  axis [description]
	 * @return      [description]
	 */
		static float GetAxis(Axis axis);
	/**
	 * Get the current axis input, sans smoothing
	 * @param  axis the axis to read
	 * @return      Axis value, between -1 and 1
	 */
		static float GetAxisRaw(Axis axis);
	/**
	 * Check to see if a button is pressed down.
	 * A button that's been pressed and not released returns true.
	 * @param  button Button to read from
	 * @return        A button
	 */
		static bool GetButtonDown(Button button);
	/**
	 * Check to see if a button is allowed up
	 * A button that's just been released 
	 * @param  button The button to check
	 * @return        The button status.
	 */
		static bool GetButtonUp(Button button);
	/**
	 * Return if a button was pressed.
	 * If a button is held, this will return false the next frame.
	 * @param  button the button pressed
	 * @return        true if the button was pressed down.
	 */
		static bool GetButton(Button button);

		static void Update(float dt);

	private:
		static std::shared_ptr<InputService> _service;


	};
}
