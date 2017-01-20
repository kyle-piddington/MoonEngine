#include "EngineApp.h"
using namespace MoonEngine;
//Static library
//(Refactor later)
Library EngineApp::AssetLibrary;

EngineApp::EngineApp(GLFWwindow * window, MoonEngineCfg config):
window(window)
{
	AssetLibrary.Init(config.assetPath);
	//Other app setup code, install callbacks etc.
	
}

EngineApp::~EngineApp()
{

}
