/**
 * Load an OBJ model and read it into an interchange mesh format.
 */

#include "GL/GLBuffer.h"
#include "GL/GLVertexArrayObject.h"
#include "GL/GLConstants.h"
#include "MeshInfo.h"
#include <string>
#include "thirdparty/tiny_obj_loader.h"
namespace MoonEngine
{
	class BasicLoader
	{
	public:
		static bool LoadIntoBuffer(std::string fileName,
			 GLBuffer * vertexBuffer,
			 GLBuffer * indexBuffer,
			 GLVertexArrayObject * vao,
			 MeshInfo * outInfo,
			 bool smooth = false);
	private:
		static unsigned loadFlatShade(std::vector<float> * vertexData, std::vector<unsigned short> * indexData,
			const  tinyobj::attrib_t & attributes, 
			const std::vector<tinyobj::shape_t> & shapes);
		static unsigned loadSmoothShade(std::vector<float> * vertexData, std::vector<unsigned short> * indexData,
			const  tinyobj::attrib_t & attributes, 
			const std::vector<tinyobj::shape_t> & shapes);

	};
};