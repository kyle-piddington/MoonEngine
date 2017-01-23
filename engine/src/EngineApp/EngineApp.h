#pragma once
#include "Loaders/Library.h"
#include "Render/I_Renderer.h"
#include "Scene/Scene.h"
#include "MoonEngineCfg.h"
#include <GLFW/glfw3.h>

namespace MoonEngine
{
	struct EngineApp
	{
	public:
		EngineApp(GLFWwindow * window, MoonEngineCfg config);
		~EngineApp();
		Scene * CreateScene();
		void run(Scene * scene, I_Renderer * renderer);
		
		
		static Library GetAssetLibrary();
	private:

		void initializeComponents(Scene * scene);
		static bool assetsLoaded;
		static Library AssetLibrary;
		GLFWwindow * window;
	};
}
