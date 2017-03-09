#include "AudioService.h"
#include "FMODAudio.h"
using namespace MoonEngine;
Audio * AudioService::_service = nullptr;
std::string AudioService::_resourcePath;
Audio * AudioService::GetAudio()
{
	if(_service == nullptr)
	{
		AudioService::Provide(new FMODAudio());
	}
	return _service;
}
