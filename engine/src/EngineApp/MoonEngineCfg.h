#pragma once

#include <string>
#include <unordered_map>

namespace MoonEngine
{
    struct MoonEngineCfg
    {
        MoonEngineCfg(std::string configName);

        std::string assetPath;

        std::unordered_map<std::string, std::string> asset_directories;

        void LoadConfigFromFile(std::string configName);

        void LoadConfig(std::string configInfo);
    };
}