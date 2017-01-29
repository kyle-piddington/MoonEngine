#pragma once
#include "MeshLibrary.h"
#include "ProgramLibrary.h"
#include "TransformLib.h"
namespace MoonEngine
{
	struct Library
	{
		static void Init(std::string resourcePath);
		static void Destory();
		static std::shared_ptr<MeshLibrary> MeshLib;
		static std::shared_ptr<ProgramLibrary> ProgramLib;
		static std::shared_ptr<TransformLibrary> TransformLib;


	};
}