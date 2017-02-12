#include "Library.h"
#include "SamplerLibrary.h"

using namespace MoonEngine;
std::shared_ptr<MeshLibrary> Library::MeshLib;
std::shared_ptr<ProgramLibrary> Library::ProgramLib;
std::shared_ptr<InstTransformLibrary> Library::InstTransformLib;
std::shared_ptr<TextureLibrary> Library::TextureLib;
std::shared_ptr<SamplerLibrary> Library::SamplerLib;
std::shared_ptr<Level> Library::LevelLib;

void Library::Init(std::string resourcePath)
{
    MeshLib = std::make_shared<MeshLibrary>(resourcePath);
    ProgramLib = std::make_shared<ProgramLibrary>(resourcePath);
    InstTransformLib = std::make_shared<InstTransformLibrary>(resourcePath);
    TextureLib = std::make_shared<TextureLibrary>(resourcePath);
    SamplerLib = std::make_shared<SamplerLibrary>();
    LevelLib = std::make_shared<Level>();
}

void Library::Destroy()
{
    MeshLib.reset();
}