#include "InstTransformLib.h"

using namespace MoonEngine;

InstTransformLibrary::InstTransformLibrary(std::string resourcePath):
    _recPath(resourcePath)
{
}

InstTransformLibrary::~InstTransformLibrary()
{
}

vector<glm::mat4> MoonEngine::InstTransformLibrary::getInstanceMatrices(std::string transformfile)
{
    std::vector<glm::mat4> transforms;

    if (_mapFileToMatrices.find(transformfile) == _mapFileToMatrices.end())
    {

        //Process file and generate vector of Mat4
        transforms = Util::InstanceLoader(_recPath + transformfile);
        //TODO: Turn the vector into a VBO
        _mapFileToMatrices[transformfile] = transforms;
    }
    return _mapFileToMatrices[transformfile];
}