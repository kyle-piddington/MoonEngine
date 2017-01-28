#pragma once
#include "InputService.h"
#include "InputConfiguration.h"
#include <memory>

namespace MoonEngine
{
	class ControllerInputService : public InputService
	{
	public:
		ControllerInputService();
		ControllerInputService(InputConfiguration cfg);

		float getAxisRaw(Axis axis);

		bool getButton(Button button);

		bool getButtonUp(Button button);

		bool getButtonDown(Button button);
	private:
		int getButtonMapping(Button button);
		InputConfiguration inputConfig;
	};
}