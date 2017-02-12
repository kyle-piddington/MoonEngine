#pragma once

#include <memory>
#include <string>
#include <vector>
#include <Component/MaterialComponents/Material.h>
#include <map>
#include <Geometry/Transform.h>

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

        struct LevelObject
        {
            std::string levelMaterial;
            Transform * transform;
        };

        void addLevelMaterial(std::string materialName, std::string mesh, Material * material, bool collider);
        void addLevelMaterial(std::string materialName, LevelMaterial levelMaterial);
        LevelMaterial * getLevelMaterial(std::string materialName);
        std::vector<std::string> getAllLevelMaterials();

        void addLevelObject(LevelObject levelObject);
        void removeLevelObject();

    private:
        map<std::string, LevelMaterial> _levelMaterials;
        vector<LevelObject> _levelObjects;
    };
}
