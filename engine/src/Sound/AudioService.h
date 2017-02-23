#pragma once
#include "Audio.h"
namespace MoonEngine
{
	class AudioService
	{
	public:
		static Audio * GetAudio();

		static void Provide(Audio * audio)
		{
			if(_service != nullptr)
			{
				_service->shutdown();
				delete _service;
			}
			audio->init();
			_service = audio;
		}
		static void SetResourcePath(std::string path)
		{
			_resourcePath = path;
		}
		static std::string GetResourcePath()
		{
			return _resourcePath;
		}

	private:
		static std::string _resourcePath;
		static Audio * _service;
	};
}