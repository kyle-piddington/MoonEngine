#pragma once

#include <memory>
#include <string>
#include "Scene/Scene.h"

class Scene;

class LevelLoader
{

    std::string LoadLevelFromFile(std::string levelName);
    void LoadLevel(std::string levelName, Scene * scene);

};