#include "Library.h"
using namespace MoonEngine;
std::shared_ptr<MeshLibrary> Library::MeshLib;
std::shared_ptr<ProgramLibrary> Library::ProgramLib;

void Library::Init(std::string resourcePath)
{
	MeshLib = std::make_shared<MeshLibrary>(resourcePath);
	ProgramLib = std::make_shared<ProgramLibrary>(resourcePath);
}
void Library::Destory()
{
	MeshLib.reset();
}