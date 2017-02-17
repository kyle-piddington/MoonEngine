#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <Component/MaterialComponents/Material.h>
#include <Geometry/Transform.h>
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include <thirdparty/rapidjson/writer.h>
#include "thirdparty/rapidjson/error/en.h"

namespace MoonEngine
{
    class Level
    {
    public:
        Level(std::string resourcePath);

        ~Level();

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

        void serializeLevelObjects();

        std::string getRecPath();

    private:
        map<std::string, LevelMaterial> _levelMaterials;
        vector<LevelObject> _levelObjects;

        std::string _recPath;

        void writeJsonFile(rapidjson::Document & document);

        rapidjson::Value serializeVec3(glm::vec3 obj, rapidjson::Document::AllocatorType& allocator);
    };
}
