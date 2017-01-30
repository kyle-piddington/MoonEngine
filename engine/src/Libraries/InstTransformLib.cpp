/*Author: Derek Nola*/

#include "InstTransformLib.h"

using namespace MoonEngine;

InstTransformLibrary::InstTransformLibrary(std::string resourcePath) :
	_recPath(resourcePath + "/")
{
}
InstTransformLibrary::~InstTransformLibrary()
{
}
GLVertexArrayObject* InstTransformLibrary::getMatrixVAO(std::string transformfile) {
	std::vector<glm::mat4>transforms;

	if (_mapFileToVAO.find(transformfile) == _mapFileToVAO.end()) {

		//Process file and generate vector of Mat4
		transforms = Util::InstanceLoader(transformfile);
		//TODO: Turn the vector into a VBO and return the VAO
	}

	return _mapFileToVAO[transformfile];
}
