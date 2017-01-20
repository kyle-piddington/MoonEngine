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
		MeshLibrary(std::string resourcePath);
		~MeshLibrary();
		public:
			MeshInfo * getInfoForMeshNamed(std::string mesh);
		private:
			std::unordered_map<std::string, MeshInfo * > mapMeshToVAO;
			std::vector<GLVertexArrayObject> meshVAOs;
	};
}