#pragma once
/**
 * ProgramLibrary provides a mapping of 
 * mesh names to GLSL programs
 */
#include <string>
#include <unordered_map>
#include <vector>
#include "GLWrapper/GLProgram.h"
#include "Loaders/ProgramInfo.h"
#include <memory>

namespace MoonEngine
{

    class ProgramLibrary
    {
    public:
        ProgramLibrary(std::string resourcePath);

        ~ProgramLibrary();

        GLProgram * getProgramForName(std::string progName);

    private:
        void loadDefaultProgram();

        std::string _recPath;
        std::unordered_map<std::string, GLProgram *> _mapProgToInfo;
        std::vector<std::shared_ptr<GLProgram>> _programs;

    };
}