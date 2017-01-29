#pragma once
#include "MeshLibrary.h"
#include "ProgramLibrary.h"
#include "TransformLib.h"
namespace MoonEngine
{
	struct Library
	{
		static void Init(std::string resourcePath);
		static void Destroy();
		static std::shared_ptr<MeshLibrary> MeshLib;
		static std::shared_ptr<ProgramLibrary> ProgramLib;
<<<<<<< HEAD:engine/src/Loaders/Library.h
		static std::shared_ptr<TransformLibrary> TransformLib;


=======
>>>>>>> origin/master:engine/src/Libraries/Library.h
	};
}
