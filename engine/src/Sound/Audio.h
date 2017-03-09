#pragma once
#include <string>
namespace MoonEngine
{
	class Audio
	{
	public:
		virtual ~Audio(){};
		virtual void init() = 0;
		virtual void shutdown() = 0;
		virtual void loadSound(std::string soundName, bool asStream, bool loop = false) = 0;
		virtual void playSound(std::string soundName) = 0;
		virtual void stopAllSounds() = 0;


	};	
}
