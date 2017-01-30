#include "Library.h"

using namespace MoonEngine;
std::shared_ptr<MeshLibrary> Library::MeshLib;
std::shared_ptr<ProgramLibrary> Library::ProgramLib;
std::shared_ptr<InstTransformLibrary> Library::InstTransformLib;
std::shared_ptr<TextureLibrary> Library::TextureLib;
void Library::Init(std::string resourcePath)
{
	MeshLib = std::make_shared<MeshLibrary>(resourcePath);
	ProgramLib = std::make_shared<ProgramLibrary>(resourcePath);
	InstTransformLib = std::make_shared<InstTransformLibrary>(resourcePath);
	TextureLib = std::make_shared<TextureLibrary>(resourcePath);

}
void Library::Destroy()
{
	MeshLib.reset();
}