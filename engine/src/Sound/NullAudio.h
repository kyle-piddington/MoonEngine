#pragma once
#include "Audio.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <vector>
#include <thread>
#include <unordered_map>
#include <deque>
#include <atomic>
namespace MoonEngine
{
	class NullAudio : public Audio
	{

		void init() {};
		void shutdown() {};
		void loadSound(std::string soundName, bool asStream, bool looping) {};
		void playSound(std::string soundName, float volume) {};
		void stopAllSounds() {};



	};
}
#pragma once
