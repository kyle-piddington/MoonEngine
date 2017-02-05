#pragma once

#include "Libraries/Library.h"
#include "Render/I_Renderer.h"
#include "Scene/Scene.h"
#include "MoonEngineCfg.h"
#include <GLFW/glfw3.h>

namespace MoonEngine
{
    struct EngineApp
    {
    public:
        EngineApp(GLFWwindow * window);

        EngineApp(GLFWwindow * window, string config);

        ~EngineApp();

        Scene * CreateScene();

        void run(Scene * scene, I_Renderer * renderer);


        static Library GetAssetLibrary();

        static Scene * GetScene();

        void setMouseMode(int mode);

    private:

        void initializeComponents(Scene * scene);

        static bool assetsLoaded;
        static Library AssetLibrary;
        static Scene * _activeScene;
        GLFWwindow * _window;
    };
}
