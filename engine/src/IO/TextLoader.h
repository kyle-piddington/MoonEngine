#pragma once

#include <string>

namespace MoonEngine
{
    namespace TextLoader
    {
        /**
  * Load a full file into a string
  * This method is meant to be used with small ASCII files, such as shaders.
  * @param 	fileName	the path to the file
  * @return	a string containing the full contents of a file.
  */
        std::string LoadFullFile(std::string fileName);
    }
}