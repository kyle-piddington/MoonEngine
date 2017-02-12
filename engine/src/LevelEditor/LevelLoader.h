#pragma once

#include <string>
#include "Scene/Scene.h"

namespace MoonEngine
{
    class LevelLoader
    {

    public:
        std::string LoadLevelFromFile(std::string levelName);

        void LoadLevel(std::string levelName, Scene * scene);
    private:

    };
}