#pragma once

#include <memory>
#include <string>
#include <vector>
#include <Component/MaterialComponents/Material.h>
#include <map>

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
        std::vector<std::string> getAllLevelMaterials();

    private:
        map <std::string, LevelMaterial> _levelMaterials;

    };
}
