#include "ProgramLibrary.h"
#include "Loaders/ProgramLoader.h"

using namespace MoonEngine;

ProgramLibrary::ProgramLibrary(std::string resourcePath):
    _recPath(resourcePath + "/")
{
    //Add a fallback program to the library.
    loadDefaultProgram();
}

ProgramLibrary::~ProgramLibrary()
{
    _mapProgToInfo.clear();
    _programs.clear();
}

GLProgram * ProgramLibrary::getProgramForName(std::string progName)
{
    if (_mapProgToInfo.find(progName) == _mapProgToInfo.end())
    {
        std::shared_ptr<GLProgram> programPtr =
                ProgramLoader::LoadProgramFromFile(_recPath + progName);
        if (programPtr != nullptr)
        {
            _programs.push_back(programPtr);
            _mapProgToInfo[progName] = programPtr.get();
        }
        else
        {
            _mapProgToInfo[progName] = _mapProgToInfo["default.program"];
        }
    }
    return _mapProgToInfo[progName];
}

void ProgramLibrary::loadDefaultProgram()
{
    std::string vShader =
            "#version 330 core\n\
	layout (location = 0) in vec4 position;\n\
	uniform mat4 M;\n\
	uniform mat4 V;\n\
	uniform mat4 P;\n\
	void main()\n\
	{\n\
		gl_Position = P * V * M *  position;\n\
	}";

    std::string fShader =
            "#version 330 core\n\
	uniform vec3 tint;\n\
	out vec4 color;\n\
	void main()\n\
	{\n\
		color = vec4(tint, 1.0);\n\
	}";

    GLShader glVShader(GL_VERTEX_SHADER, vShader.c_str());
    GLShader glFShader(GL_FRAGMENT_SHADER, fShader.c_str());

    std::shared_ptr<GLProgram> defaultProg = std::make_shared<GLProgram>("default");
    defaultProg->attachShader(glVShader);
    defaultProg->attachShader(glFShader);
    defaultProg->link();
    _programs.push_back(defaultProg);
    _mapProgToInfo["default.program"] = defaultProg.get();

}