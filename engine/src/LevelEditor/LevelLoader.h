#pragma once

#include <memory>
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
        struct LevelMaterial
        {
            std::string mesh;
            Material * material;
            bool collider;
        };

        unordered_map<std::string, LevelMaterial> _levelMaterials;
    };
}