#pragma once

/**
 * Wrapper to hold on to GLFW controller inputs.
 */
namespace MoonEngine
{
	class Controller
	{
	private:
		static short * _btnStatus;
  		static bool  * _btnToggles;
  		const static float * _axisStatus;
  		static int _numButtons;
  		static int _numAxes;
  		static int _joyID;
  		static bool _connected;

  		static void resetStorage();
  	public:
  		/**
  		 * Connect and configure a contorller
  		 * @param joyID the ID of the controller
  		 */
  		static void Connect(int joyID);

  		static void Disconnect();

  		static bool IsConnected();

  		static void Update();


  		static float GetAxis(unsigned axisNumber);
		/**
		 * Check if a button was just pressed down
		 * @param  buttonNumber the button number
		 * @return              true if the button was down for one frame.
		 */
  		static bool IsButtonDown(unsigned buttonNumber);
  		/**
  		 * Check if a button was just released
  		 * @param  buttonNumber the button number
  		 * @return              true if the button was just released
  		 */
		static bool IsButtonUp(unsigned buttonNumber);
		/**
		 * Return true if a button was pressed or held
		 * @param  buttonNumber a button number
		 * @return              true if the button is being held
		 */
		static bool Button(unsigned buttonNumber);


  		

	};
}