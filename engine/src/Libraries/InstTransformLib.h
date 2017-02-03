#include <string>
#include <unordered_map>
#include <vector>
#include "GLWrapper/GLVertexArrayObject.h"
#include <memory>
#include "Util/Logger.h"
#include "Util/InstanceLoader.h"

namespace MoonEngine
{

    class InstTransformLibrary
    {
    public:
        InstTransformLibrary(std::string resourcePath);

        ~InstTransformLibrary();

        vector<glm::mat4> getInstanceMatrices(std::string transformfile);

    private:
        std::string _recPath;
        std::unordered_map<std::string, vector<glm::mat4>> _mapFileToMatrices;
    };
}