#include "FMODAudio.h"
#include "Util/Logger.h"
#include <iostream>
#include <cassert>
#include <deque>
#include <mutex>
#include "AudioService.h"
using namespace MoonEngine;
//Code for initialization taken from https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-part-1-build-environment-initialization-and-playing-sounds/
std::mutex __commandQueueMutex;
void FMODErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

void copyCommands(std::deque<FMODAudio::__audioCommand> * threadQueue, std::deque<FMODAudio::__audioCommand> * copyQueue)
{
	std::lock_guard<std::mutex> guard(__commandQueueMutex);
	*threadQueue = *copyQueue;
	copyQueue->clear();
}

void loadAudio(std::string audio, std::unordered_map<std::string,FMOD::Sound *> * audioMap, bool asStream, bool loop, FMOD::System * system)
{
	std::cout << "Loading Audio" << std::endl;

	if(audioMap->find(audio) != audioMap->end())
	{
		return;
	}
	else
	{
		(*audioMap)[audio] = nullptr;
		FMOD_MODE mode = FMOD_DEFAULT;
		if(asStream)
		{
			mode |= FMOD_CREATESTREAM;
		}
		if(loop)
		{
			mode |= FMOD_LOOP_NORMAL;
		}
		FMOD_RESULT result;
			result = 
				system->createSound(
					(AudioService::GetResourcePath() + audio).c_str(), 
					mode, 0, &((*audioMap)[audio]));
		FMODErrorCheck(result);
	}

}

void playSound(std::string audio, std::unordered_map<std::string,FMOD::Sound *>  & audioMap, FMOD::System * system)
{
	//Debug checking
	std::cout << "Playing Audio" << std::endl;
	assert(audioMap.find(audio) != audioMap.end());
	//Play sound.
	system->playSound(audioMap[audio],0,false,0);
}


void runThread(std::atomic<bool> * running, std::deque<FMODAudio::__audioCommand> * commandQueue, FMOD::System * system)
{

	std::unordered_map<std::string,FMOD::Sound *> audio;
	std::deque <FMODAudio::__audioCommand> audioQueue;
	while(*running)
	{
		copyCommands(&audioQueue, commandQueue);
		while(!audioQueue.empty())
		{
			auto cmd = audioQueue.front();
			switch(cmd.type)
			{
				case FMODAudio::LOAD:
					loadAudio(cmd.commandStrParam, &audio, false,false,system);
					break;
				case FMODAudio::LOADSTR:
					loadAudio(cmd.commandStrParam, &audio, true,false,system);
					break;
				case FMODAudio::LOADLOOP:
					loadAudio(cmd.commandStrParam, &audio, false,true,system);
					break;
				case FMODAudio::LOADSTRLOOP:
					loadAudio(cmd.commandStrParam, &audio, true,true,system);
					break;
				case FMODAudio::PLAY:
					playSound(cmd.commandStrParam,audio,system);
					break;
				
				default:
					break;
			}
			audioQueue.pop_front();
		}
		system->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

 	}
}

void FMODAudio::init()
{
	//Initialize audio
	//Start a thread
	FMOD_RESULT result;
	unsigned int version;
	

	result = FMOD::System_Create(&system);
	FMODErrorCheck(result);
	result = system->getVersion(&version);
	FMODErrorCheck(result);

	if (version < FMOD_VERSION)
	{
		LOG(FATAL_ERROR, "You are using an old version of FMOD " +
			std::to_string(version) + ". This program requires " + std::to_string(FMOD_VERSION));

		assert(!"FMod version error");
	}
	std::cout << "Initting Audio" << std::endl;
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	FMODErrorCheck(result);
	//Audio thread starts at this point.
	_isRunning = true;
	_audioThread = std::thread(runThread, &_isRunning, &commandQueue, system);


}





void FMODAudio::stopAllSounds()
{

}

void FMODAudio::pushCommand(__audioCommand cmd)
{
	std::lock_guard<std::mutex> guard(__commandQueueMutex);
	commandQueue.push_back(cmd);
}

void FMODAudio::loadSound(std::string soundName,  bool asStream, bool loop)
{
	__audioCommand cmd;
	cmd.commandStrParam = soundName;
	if(asStream)
	{
		if(!loop)
		{
			cmd.type = LOADSTR;			
		}
		else
		{
			cmd.type = LOADSTRLOOP;
		}

	}
	else
	{
		if(loop)
		{
			cmd.type = LOADLOOP;
		}
		else
		{
			cmd.type = LOAD;			
		}

	}
	pushCommand(cmd);
}

void FMODAudio::playSound(std::string soundName)
{
	__audioCommand cmd;
	cmd.commandStrParam = soundName;
	cmd.type = PLAY;
	
	pushCommand(cmd);
}


void FMODAudio::shutdown()
{
	stopAllSounds();
	_isRunning = false;
	_audioThread.join();
}