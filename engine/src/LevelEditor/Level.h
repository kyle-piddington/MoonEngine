#pragma once

#include <memory>
#include <string>
#include <Component/MaterialComponents/Material.h>

namespace MoonEngine
{
    class Level
    {
    public:
        struct LevelMaterial
        {
            std::string mesh;
            Material * material;
            bool collider;
        };

        void addLevelMaterial(std::string materialName, std::string mesh, Material * material, bool collider);
        void addLevelMaterial(std::string materialName, LevelMaterial levelMaterial);
        LevelMaterial * getLevelMaterial(std::string materialName);

    private:
        unordered_map <std::string, LevelMaterial> _levelMaterials;

    };
}
