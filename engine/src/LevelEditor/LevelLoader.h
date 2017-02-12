#pragma once

#include <string>
#include "Scene/Scene.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/error/error.h"
#include "thirdparty/rapidjson/error/en.h"

namespace MoonEngine
{
    class LevelLoader
    {

    public:
        void LoadLevel(std::string levelName, Scene * scene);
        
    private:
        void LoadLevelMaterials(const rapidjson::Document & document, Scene * scene);
        void LoadLevelObjects(const rapidjson::Document & document, Scene * scene);

    };
}