#include "Library.h"
using namespace MoonEngine;
std::shared_ptr<MeshLibrary> Library::MeshLib;
std::shared_ptr<ProgramLibrary> Library::ProgramLib;
std::shared_ptr<InstTransformLibrary> Library::InstTransformLib;
void Library::Init(std::string resourcePath)
{
	MeshLib = std::make_shared<MeshLibrary>(resourcePath);
	ProgramLib = std::make_shared<ProgramLibrary>(resourcePath);
	InstTransformLib = std::make_shared<InstTransformLibrary>(resourcePath);
}
void Library::Destroy()
{
	MeshLib.reset();
}