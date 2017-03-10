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
	class FMODAudio : public Audio
	{
	public:
		~FMODAudio(){}
		enum __audioCommandType
		{
			LOAD,
			LOADSTR,
			LOADLOOP,
			LOADSTRLOOP,
			PLAY,
			STOP,
			STOPALL
		};

		struct __audioCommand
		{
			__audioCommandType type;
			std::string commandStrParam;
		};

		void init();
		void shutdown();
		void loadSound(std::string soundName, bool asStream, bool looping);
		void playSound(std::string soundName);
		void stopAllSounds();
	private:
		std::atomic<bool> _isRunning;
		FMOD::System *system;

		
		std::deque <__audioCommand> commandQueue;
		std::thread _audioThread;

		//Thread functions
		
		//Threadsave accessors to accessing audio commands
		void pushCommand(__audioCommand cmd);
		
		

	};	
}
