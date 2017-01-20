#pragma once
#include "MeshLibrary.h"
namespace MoonEngine
{
	struct Library
	{
		static void Init(std::string resourcePath);
		static void Destory();
		static MeshLibrary MeshLibrary;


	};
}
