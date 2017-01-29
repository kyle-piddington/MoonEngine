#include <string>
#include <unordered_map>
#include <vector>
#include "GL/GLVertexArrayObject.h"
#include <memory>
#include "BasicLoader.h"
#include "Util/Logger.h"
#include "Util\InstanceLoader.h"

namespace MoonEngine
{

	class TransformLibrary
	{
	public:
		TransformLibrary(std::string resourcePath);
		~TransformLibrary();
		GLVertexArrayObject* getMatrixVAO(std::string transformfile);
	private:
		std::string _recPath;
		std::unordered_map<std::string, GLVertexArrayObject*> _mapFileToVAO;
	};
}