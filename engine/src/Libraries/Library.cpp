#include "Library.h"
#include "SamplerLibrary.h"
#include <iostream>
#include <unordered_map>

using namespace MoonEngine;

std::shared_ptr<MeshLibrary> Library::MeshLib;
std::shared_ptr<ProgramLibrary> Library::ProgramLib;
std::shared_ptr<InstTransformLibrary> Library::InstTransformLib;
std::shared_ptr<TextureLibrary> Library::TextureLib;
std::shared_ptr<SamplerLibrary> Library::SamplerLib;
std::shared_ptr<Level> Library::LevelLib;

void Library::Init(MoonEngineCfg config)
{
    string resourcePath = config.assetPath;
    std::unordered_map<std::string, std::string> dirs = config.asset_directories;

    MeshLib = std::make_shared<MeshLibrary>(resourcePath + dirs["meshes"]);
    ProgramLib = std::make_shared<ProgramLibrary>(resourcePath + dirs["programs"]);
    InstTransformLib = std::make_shared<InstTransformLibrary>(resourcePath + dirs["instances"]);
    TextureLib = std::make_shared<TextureLibrary>(resourcePath + dirs["textures"]);
    SamplerLib = std::make_shared<SamplerLibrary>();
    LevelLib = std::make_shared<Level>(resourcePath);
}

void Library::Destroy()
{
    MeshLib.reset();
}