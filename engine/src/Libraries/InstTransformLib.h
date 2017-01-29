#include <string>
#include <unordered_map>
#include <vector>
#include "GL/GLVertexArrayObject.h"
#include <memory>
#include "Util/Logger.h"
#include "Util\InstanceLoader.h"

namespace MoonEngine
{

	class InstTransformLibrary
	{
	public:
		InstTransformLibrary(std::string resourcePath);
		~InstTransformLibrary();
		GLVertexArrayObject* getMatrixVAO(std::string transformfile);
	private:
		std::string _recPath;
		std::unordered_map<std::string, GLVertexArrayObject*> _mapFileToVAO;
	};
}