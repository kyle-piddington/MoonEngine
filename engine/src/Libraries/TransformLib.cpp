#include "TransformLib.h"

using namespace MoonEngine;

TransformLibrary::TransformLibrary(std::string resourcePath) :
	_recPath(resourcePath + "/")
{
}
TransformLibrary::~TransformLibrary()
{
}
GLVertexArrayObject* TransformLibrary::getMatrixVAO(std::string transformfile) {
	std::vector<glm::mat4>transfroms;

	if (_mapFileToVAO.find(transformfile) == _mapFileToVAO.end()) {

		//Process file and generate vector of Mat4
		Util::Inst

	}

	return _mapFileToVAO[transformfile];
}
]