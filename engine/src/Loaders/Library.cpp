#include "Library.h"
using namespace MoonEngine;
std::shared_ptr<MeshLibrary> Library::MeshLib;

void Library::Init(std::string resourcePath)
{
	MeshLib = std::make_shared<MeshLibrary>(resourcePath);
}
void Library::Destory()
{
	MeshLib.reset();
}