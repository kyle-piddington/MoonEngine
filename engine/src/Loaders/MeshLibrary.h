#pragma once
/**
 * MeshLibrary provides a mapping of 
 * mesh names to vertex array objects. 
 */
#include <string>
#include <unordered_map>
#include <vector>
#include "GL/GLVertexArrayObject.h"
#include "MeshInfo.h"

namespace MoonEngine
{

	class MeshLibrary
	{
		public:
		MeshLibrary(std::string resourcePath);
		~MeshLibrary();
			MeshInfo * getInfoForMeshNamed(std::string mesh);
		private:
			std::string _recPath;
			std::unordered_map<std::string, MeshInfo * > _mapMeshToInfo;
			std::vector<GLVertexArrayObject> _meshVAOs;
			std::vector<GLBuffer> _meshBuffers;
	};
}