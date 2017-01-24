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
#include <memory>
namespace MoonEngine
{

	class MeshLibrary
	{
		public:
		MeshLibrary(std::string resourcePath);
		~MeshLibrary();
			MeshInfo * getInfoForMeshNamed(std::string mesh, bool smooth = false);
		private:
			std::string _recPath;
			std::unordered_map<std::string, MeshInfo * > _mapMeshToInfo;
			std::vector<std::shared_ptr<GLVertexArrayObject>> _meshVAOs;
			std::vector<std::shared_ptr<GLBuffer>> _meshBuffers;
	};
}